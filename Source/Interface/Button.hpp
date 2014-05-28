#pragma once

#include "Precompiled.hpp"
#include "InterfaceElement.hpp"

#include "Common/Dispatcher.hpp"

//
// Button
//

class Button : public InterfaceElement
{
public:
    struct EventAction
    {
    };

    struct EventPressed
    {
        EventPressed(bool pressed) :
            pressed(pressed) { }

        bool pressed;
    };

    struct EventHovered
    {
        EventHovered(bool hovered) :
            hovered(hovered) { }

        bool hovered;
    };

public:
    Button();
    virtual ~Button();

    void Cleanup();

    virtual bool Process(const SDL_Event& event);
    virtual void Update(float timeDelta);
    //virtual void Draw() = 0;

    void Enable();
    void Disable();

    void SetEnabled(bool enabled);
    void SetText(std::string text);
    void SetPosition(const glm::vec2& position);
    void SetSize(const glm::vec2& size);

    const std::string& GetText() const;
    const glm::vec2& GetPosition() const;
    const glm::vec2& GetSize() const;

    bool IsEnabled() const;
    bool IsHovered() const;
    bool IsPressed() const;

public:
    void OnEventAction(const ReceiverSignature<EventAction>& receiver);
    void OnEventHovered(const ReceiverSignature<EventHovered>& receiver);
    void OnEventPressed(const ReceiverSignature<EventPressed>& receiver);

private:
    // Button state.
    std::string m_text;
    glm::vec2 m_position;
    glm::vec2 m_size;

    bool m_enabled;
    bool m_hovered;
    bool m_pressed;

    // Event dispatchers.
    Dispatcher<EventAction> m_eventAction;
    Dispatcher<EventPressed> m_eventPressed;
    Dispatcher<EventHovered> m_eventHovered;
};
