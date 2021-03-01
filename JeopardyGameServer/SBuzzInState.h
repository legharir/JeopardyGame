#pragma once

#include "SGameState.h"
#include "GameMessage.h"

class SBuzzInState : public SGameState
{
public:
    void init(Game* game);
    void update();

    void handleBuzzIn(const Player& player);

    // Singleton class shouldn't be copyable or deletable.
    SBuzzInState(SBuzzInState& other) = delete;
    void operator=(const SBuzzInState&) = delete;

    static SBuzzInState* getInstance()
    {
        return &m_buzzInState;
    }

protected:
    SBuzzInState()
    {
    }

private:
    static SBuzzInState m_buzzInState;
    Game* m_game;
};
