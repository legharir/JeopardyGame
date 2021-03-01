#pragma once

#include "SGameState.h"
#include "GameMessage.h"

#include <map>

class SWagerState : public SGameState
{
public:
    void init(Game* game);
    void update();

    void handleWager(const WagerMessage& message, const Player& player);

    // Singleton class shouldn't be copyable or deletable.
    SWagerState(SWagerState& other) = delete;
    void operator=(const SWagerState&) = delete;

    static SWagerState* getInstance()
    {
        return &m_sWagerState;
    }

protected:
    SWagerState()
    {
    }

private:
    static SWagerState m_sWagerState;
    Game* m_game;
};
