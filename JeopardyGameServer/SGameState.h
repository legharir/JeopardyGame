#pragma once

#include "Game.h"
#include "GameMessage.h"

class SGameState
{
public:
    virtual void init(Game* game) = 0;
    virtual void update(){};

    virtual void handlePlayerJoined(const PlayerJoinedMessage& message, sf::TcpSocket* socket){};
    virtual void handleGameStart(const GameStartMessage& message){};
    virtual void handleCluePicked(const CluePickedMessage& message, const Player& player){};
    virtual void handleWager(const WagerMessage& message, const Player& player){};
    virtual void handleBuzzIn(const Player& player){};
    virtual void handlePartialClueResponse(const PartialClueResponseMessage& message, const Player& player){};
    virtual void handleClueResponse(const ClueResponseMessage& message, const Player& player){};

    void changeState(Game* game, SGameState* state)
    {
        game->changeState(state);
    }

protected:
    SGameState()
    {
    }
};
