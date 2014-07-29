#pragma once

#include "Precompiled.hpp"

// Forward declarations.
class Logger;
class CacheManager;
class ConsoleSystem;
class ConsoleHistory;
class ConsoleFrame;
class ScreenSpace;
class CoreRenderer;
class BasicRenderer;
class TextRenderer;
class FrameCounter;

class Texture;
class Font;

class BaseFrame;

//
// Console Definitions
//

namespace Console
{
    // Window settings.
    extern ConsoleVariable windowName;
    extern ConsoleVariable windowWidth;
    extern ConsoleVariable windowHeight;
    extern ConsoleVariable windowResize;

    // Render settings.
    extern ConsoleVariable renderVsync;
}

//
// Main Context
//

namespace Main
{
    // Context methods.
    bool Initialize();
    void Cleanup();
    void Quit();

    // Context accessors.
    bool            IsInitialized();
    bool            IsQuitting();

    std::string     GetCurrentDir();
    std::string     GetSourceDir();
    std::string     GetWorkingDir();
    std::string     GetCacheDir();

    Logger&         GetLogger();
    CacheManager&   GetCacheManager();
    ConsoleSystem&  GetConsoleSystem();
    ConsoleHistory& GetConsoleHistory();
    ConsoleFrame&   GetConsoleFrame();
    ScreenSpace&    GetScreenSpace();
    CoreRenderer&   GetCoreRenderer();
    BasicRenderer&  GetBasicRenderer();
    TextRenderer&   GetTextRenderer();
    FrameCounter&   GetFrameCounter();

    Texture&        GetBlankTexture();
    Font&           GetDefaultFont();

    SDL_Window*     GetSystemWindow();
    SDL_GLContext   GetGraphicsContext();
    FT_Library      GetFontLibrary();

    BaseFrame&      GetMainFrame();
}
