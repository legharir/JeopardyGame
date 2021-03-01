#pragma once

#include "GameMessage.h"
#include "GameState.h"

#include "Button.h"
#include "TextField.h"

#include <SFML/Graphics.hpp>

class LobbyState : public GameState
{

public:
    void init(Engine* game);

    void handleEvent(const sf::Event& event);

    void handlePlayerJoined(const PlayerJoinedMessage& message);
    void handleGameStart(const GameStartMessage& message);

    void update();
    void draw(sf::RenderWindow& window);

    // Singleton class shouldn't be copyable or deletable
    LobbyState(LobbyState& other) = delete;
    void operator=(const LobbyState&) = delete;

    static LobbyState* getInstance()
    {
        return &m_lobbyState;
    }

protected:
    LobbyState()
    {
    }

private:
    Engine* m_game;

    static LobbyState m_lobbyState;

    // Event handlers.
    void handleMouseButtonPressed(const sf::Event::MouseButtonEvent& mouse);
    void handleTextEntered(const sf::Event::TextEvent& text);

    // UI elements.
    sf::Font m_font;
    Button m_startGameButton;
    TextField m_seasonField;
    TextField m_episodeField;
};
