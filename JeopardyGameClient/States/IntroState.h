#pragma once

#include "GameState.h"

#include "Button.h"
#include "TextField.h"

#include <SFML/Graphics.hpp>

class IntroState : public GameState
{

public:
    void init(Engine* game);

    void handleEvent(const sf::Event& event);

    void update();
    void draw(sf::RenderWindow& window);

    // Singleton class shouldn't be copyable or deletable.
    IntroState(IntroState& other) = delete;
    void operator=(const IntroState&) = delete;

    static IntroState* getInstance()
    {
        return &m_introState;
    }

protected:
    IntroState()
    {
    }

private:
    static IntroState m_introState;

    Engine* m_game;

    // Event handlers.
    void handleMouseButtonPressed(const sf::Event::MouseButtonEvent& mouse);
    void handleTextEntered(const sf::Event::TextEvent& text);

    // UI elements.
    TextField m_nameField;
    Button m_joinButton;
};
