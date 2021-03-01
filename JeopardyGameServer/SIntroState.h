#pragma once

#include "SGameState.h"
#include "GameMessage.h"

class SIntroState : public SGameState
{
public:
    void init(Game* game);

    void handlePlayerJoined(const PlayerJoinedMessage& message, sf::TcpSocket* socket);
    void handleGameStart(const GameStartMessage& message);

    // Singleton class shouldn't be copyable or deletable.
    SIntroState(SIntroState& other) = delete;
    void operator=(const SIntroState&) = delete;

    static SIntroState* getInstance()
    {
        return &m_sIntroState;
    }

protected:
    SIntroState()
    {
    }

private:
    static SIntroState m_sIntroState;
    Game* m_game;
};
