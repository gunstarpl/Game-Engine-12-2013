#include "Precompiled.hpp"
#include "LuaScript.hpp"

namespace
{
    // Log error messages.
    #define LogLoadError(filename) "Failed to load \"" << filename << "\" file! "
}

LuaScript::LuaScript() :
    m_state(nullptr),
    m_level(0)
{
}

LuaScript::LuaScript(std::string filename) :
    LuaScript()
{
    Load(filename);
}

LuaScript::~LuaScript()
{
    Cleanup();
}

bool LuaScript::Load(std::string filename)
{
    Cleanup();

    // Initialization state.
    bool initialized = false;

    // Create new Lua state.
    m_state = luaL_newstate();
    SCOPE_GUARD_IF(!initialized, Cleanup());

    // Load Lua file.
    if(luaL_loadfile(m_state, filename.c_str()) != 0)
    {
        Log() << LogLoadError(filename) << "Couldn't parse the file.";
        return false;
    }

    // Execute Lua state once.
    if(lua_pcall(m_state, 0, 0, 0) != 0)
    {
        Log() << LogLoadError(filename) << "Couldn't execute the state.";
        return false;
    }

    // Success!
    return initialized = true;
}

void LuaScript::Cleanup()
{
    if(m_state)
    {
        lua_close(m_state);
        m_state = nullptr;
    }

    m_level = 0;
}

std::string LuaScript::DetachStem(std::string& compound)
{
    // Find the separator.
    std::size_t separator = compound.find_first_of('.');

    // Copy the stem.
    std::string detached(compound, 0, separator);

    // Erase detached stem.
    if(separator != std::string::npos)
    {
        compound.erase(0, separator + 1);
    }
    else
    {
        compound.erase();
    }

    return detached;
}

bool LuaScript::PushVariable(std::string compound)
{
    assert(m_state);

    // Push compound on the stack.
    std::string element;

    while(true)
    {
        // Detach element name from variable compound.
        element = DetachStem(compound);

        if(element.empty())
            break;

        // Push variable on the stack.
        if(m_level == 0)
        {
            lua_getglobal(m_state, element.c_str());
        }
        else
        {
            lua_getfield(m_state, -1, element.c_str());
        }

        m_level++;

        // Check if we pushed nil.
        if(lua_isnil(m_state, -1))
            return false;
    }

    return true;
}

void LuaScript::PopStack()
{
    assert(m_state);

    // Pop the entire stack.
    lua_pop(m_state, m_level);
    m_level = 0;
}

std::string LuaScript::GetString(std::string compound, std::string unavailable)
{
    std::string value = unavailable;

    if(m_state)
    {
        if(PushVariable(compound))
        {
            if(lua_isstring(m_state, -1))
            {
                value = lua_tostring(m_state, -1);
            }
            
            PopStack();
        }
    }

    return value;
}

bool LuaScript::GetBool(std::string compound, bool unavailable)
{
    bool value = unavailable;

    if(m_state)
    {
        if(PushVariable(compound))
        {
            if(lua_isboolean(m_state, -1))
            {
                value = lua_toboolean(m_state, -1) != 0;
            }
            
            PopStack();
        }
    }

    return value;
}

int LuaScript::GetInteger(std::string compound, int unavailable)
{
    int value = unavailable;

    if(m_state)
    {
        if(PushVariable(compound))
        {
            if(lua_isnumber(m_state, -1))
            {
                value = (int)lua_tonumber(m_state, -1);
            }
            
            PopStack();
        }
    }

    return value;
}

float LuaScript::GetFloat(std::string compound, float unavailable)
{
    float value = unavailable;

    if(m_state)
    {
        if(PushVariable(compound))
        {
            if(lua_isnumber(m_state, -1))
            {
                value = (float)lua_tonumber(m_state, -1);
            }
            
            PopStack();
        }
    }

    return value;
}

bool LuaScript::IsValid() const
{
    return m_state != nullptr;
}
