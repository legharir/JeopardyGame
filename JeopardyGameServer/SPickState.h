#pragma once

#include "SGameState.h"
#include "GameMessage.h"

class SPickState : public SGameState
{
public:
    void init(Game* game);
    void update();

	void handleCluePicked(const CluePickedMessage& message, const Player& player);

    // Singleton class shouldn't be copyable or deletable.
    SPickState(SPickState& other) = delete;
    void operator=(const SPickState&) = delete;

    static SPickState* getInstance()
    {
        return &m_sPickState;
    }

protected:
    SPickState()
    {
    }

private:
    static SPickState m_sPickState;
    Game* m_game;

    long long m_cluePickedTimestamp;
};
