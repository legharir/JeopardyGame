#pragma once

#include "SGameState.h"
#include "GameMessage.h"

class SEndGameState : public SGameState
{
public:
    void init(Game* game);
    
    // Singleton class shouldn't be copyable or deletable.
    SEndGameState(SEndGameState& other) = delete;
    void operator=(const SEndGameState&) = delete;

    static SEndGameState* getInstance()
    {
        return &m_sEndGameState;
    }

protected:
    SEndGameState()
    {
    }

private:
    static SEndGameState m_sEndGameState;
    Game* m_game;
};

