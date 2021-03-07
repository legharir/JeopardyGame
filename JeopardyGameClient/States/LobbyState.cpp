#include "LobbyState.h"

#include "PickState.h"
#include "Utils.h"
#include "ResourcePath.hpp"

#include <SFML/Network.hpp>
#include <iostream>

LobbyState LobbyState::m_lobbyState;

void LobbyState::init(Engine* game)
{
    std::cout << "[Client] In LobbyState" << std::endl;
    m_game = game;

    // Let the game server know that the current player just joined.
    PlayerJoinedMessage message(m_game->getSelf()->getName());
    sf::Packet packet;
    packet << message;
    m_game->getClient()->send(packet);

    if (!m_font.loadFromFile(resourcePath() + "KORIN.ttf"))
        throw;

    m_startGameButton.setPosition(sf::Vector2f(300, 500));
    m_startGameButton.setText("Start Game");
    m_startGameButton.setFont(m_font);
    m_startGameButton.setSize(sf::Vector2f(150, 40));

    m_seasonField.setPosition(sf::Vector2f(200, 200));
    m_seasonField.setLabel("Season:");
    m_seasonField.setFont(m_font);

    m_episodeField.setPosition(sf::Vector2f(200, 300));
    m_episodeField.setLabel("Episode:");
    m_episodeField.setFont(m_font);
}

void LobbyState::handleEvent(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::MouseButtonPressed:
            handleMouseButtonPressed(event.mouseButton);
            break;
        case sf::Event::TextEntered:
            handleTextEntered(event.text);
            break;
    }
}

void LobbyState::handlePlayerJoined(const PlayerJoinedMessage& message)
{
    std::cout << "New player joined: " << message.playerName << std::endl;
    m_game->addPlayer(message.playerName);
}

void LobbyState::handleGameStart(const GameStartMessage& message)
{
    changeState(m_game, PickState::getInstance());
}

void LobbyState::update()
{
}

void LobbyState::draw(sf::RenderWindow& window)
{
    float y = 0;
    for (const auto& player : m_game->getPlayers())
    {
        sf::Text playerName(player->getName(), m_font);
        playerName.setPosition(sf::Vector2f(0, y));
        window.draw(playerName);
        y += 50;
    }

    window.draw(m_seasonField);
    window.draw(m_episodeField);
    window.draw(m_startGameButton);
}

void LobbyState::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& mouse)
{
    // Check if the player pressed the start game button.
    if (m_startGameButton.clicked(mouse))
    {
        // Tell the game server to start the game.
        const auto& season = stoi(m_seasonField.getText());
        const auto& episode = stoi(m_episodeField.getText());

        GameStartMessage message(season, episode);
        sf::Packet packet;
        packet << message;
        m_game->getClient()->send(packet);
    }

    m_seasonField.onClick(mouse);
    m_episodeField.onClick(mouse);
}

void LobbyState::handleTextEntered(const sf::Event::TextEvent& text)
{
    if (text.unicode == '\t')
    {
        if (m_seasonField.isFocused())
        {
            m_seasonField.setFocused(false);
            m_episodeField.setFocused(true);
        }
        else
        {
            m_seasonField.setFocused(true);
            m_episodeField.setFocused(false);
        }

    }
    
    if (m_seasonField.isFocused())
        m_seasonField.onTextEntered(text.unicode);
    else if (m_episodeField.isFocused())
		m_episodeField.onTextEntered(text.unicode);
}
