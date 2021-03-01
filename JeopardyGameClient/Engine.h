#pragma once

#include "Client.h"
#include "DrawablePlayerBoard.h"
#include "GameMessage.h"
#include "PlayerInfo.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <queue>
#include <vector>

class GameState;

class Engine
{
public:
    Engine();

    bool isRunning();

    Client* getClient();

    // Game loop stuff. Handles SFML events like keyboard/mouse events as well
    // as game "events" like messages from the game server.
    void handleEvents();
    void update();
    void draw();
    void changeState(GameState* state);

    // Player stuff.
    PlayerInfo* getPlayerInfo();

    const Player* getSelf() const;
    std::vector<const Player*> getPlayers() const;

    const Player* getCluePicker() const;
    const Player* getClueResponder() const;

    void setCluePicker(const std::string& playerName);
    void setClueResponder(const std::string& playerName);

    void initSelf(const std::string& playerName);
    void addPlayer(const std::string& playerName);

    void updatePlayerBalance(const std::string& playerName, int delta);

    sf::Time getDeltaTime() const;
    sf::Vector2u getWindowSize() const;

private:
    bool m_running;

    // To measure and hold the delta time.
    sf::Clock m_clock;
    sf::Time m_dt;

    // Client used to communicate with the game server.
    Client m_client;

    PlayerInfo m_playerInfo;

    // The SFML window used for drawing stuff.
    sf::RenderWindow m_window;
    // Reusable union for checking SFML events.
    sf::Event m_event;

    // Stack of game states, the state at the top of the stack handles events
    // and gets to draw on the window.
    std::vector<GameState*> m_states;

    // Queue that holds the messages received from the game server. Since this
    // is a single producer single consumer scenario, I think I can get away
    // with using the non-thread safe std::queue class.
    std::queue<sf::Packet> m_messageQueue;
};
