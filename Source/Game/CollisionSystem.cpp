#include "Precompiled.hpp"
#include "CollisionSystem.hpp"

#include "TransformComponent.hpp"
#include "CollisionComponent.hpp"
#include "ScriptComponent.hpp"
#include "Script.hpp"

#include "GameContext.hpp"
#include "EntitySystem.hpp"

namespace
{
    void TransformBoundingBox(glm::vec4* boundingBox, const TransformComponent* transform)
    {
        // Transform only by position (temp).
        boundingBox->x += transform->GetPosition().x;
        boundingBox->y += transform->GetPosition().y;
        boundingBox->z += transform->GetPosition().x;
        boundingBox->w += transform->GetPosition().y;
    }

    bool IntersectBoundingBox(const glm::vec4& a, const glm::vec4& b)
    {
        // Checks if bounding boxes collide.
        return !(a.x > b.z || a.z < b.x || a.y > b.w || a.w < b.y);
    }
}

CollisionSystem::CollisionSystem()
{
}

CollisionSystem::~CollisionSystem()
{
    Cleanup();
}

bool CollisionSystem::Initialize()
{
    Cleanup();

    return true;
}

void CollisionSystem::Cleanup()
{
    ClearContainer(m_objects);
}

void CollisionSystem::Update()
{
    // Create a list of collision objects.
    for(auto it = Game::CollisionComponents().Begin(); it != Game::CollisionComponents().End(); ++it)
    {
        // Check if entity is active.
        if(!Game::EntitySystem().IsHandleValid(it->first))
            continue;

        // Get the collision component.
        CollisionComponent* collision = &it->second;

        assert(collision != nullptr);

        if(!collision->IsEnabled())
            continue;

        // Get the transform component.
        TransformComponent* transform = Game::TransformComponents().Lookup(it->first);

        if(transform == nullptr)
            continue;

        // Get the script component (not required).
        ScriptComponent* script = Game::ScriptComponents().Lookup(it->first);

        // Transform the bounding box to world space.
        glm::vec4 boundingBox = collision->GetBoundingBox();
        TransformBoundingBox(&boundingBox, transform);

        // Add a collision object.
        CollisionObject object;
        object.entity = it->first;
        object.transform = transform;
        object.collision = collision;
        object.script = script;
        object.worldAABB = boundingBox;

        m_objects.push_back(object);
    }

    // Process collision objects.
    for(auto it = m_objects.begin(); it != m_objects.end(); ++it)
    {
        // Need a script component for collision reaction.
        if(it->script == nullptr)
            continue;

        // Check if collision is still enabled.
        if(!it->collision->IsEnabled())
            continue;

        // Check if it collides with other objects.
        for(auto other = m_objects.begin(); other != m_objects.end(); ++other)
        {
            // Don't check against itself.
            if(other == it)
                continue;

            // Check if collision is still enabled.
            if(!other->collision->IsEnabled())
                continue;

            // Check for collision.
            if(IntersectBoundingBox(it->worldAABB, other->worldAABB))
            {
                // Execute object script.
                it->script->GetScript()->OnCollision(*it, *other);

                // Check if this collision object is still enabled.
                if(!it->collision->IsEnabled())
                    break;
            }
        }
    }

    // Clear intermediate collision object list.
    m_objects.clear();
}