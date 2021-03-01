#pragma once

#include "SGameState.h"
#include "GameMessage.h"

class SPlayState : public SGameState
{
public:
    void init(Game* game);
    void update();

    void handlePartialClueResponse(const PartialClueResponseMessage& message, const Player& player);
    void handleClueResponse(const ClueResponseMessage& message, const Player& player);

    // Singleton class shouldn't be copyable or deletable.
    SPlayState(SPlayState& other) = delete;
    void operator=(const SPlayState&) = delete;

    static SPlayState* getInstance()
    {
        return &m_sPlayState;
    }

protected:
    SPlayState()
    {
    }

private:
    static SPlayState m_sPlayState;
    Game* m_game;

    long long m_responseDeadlineExceededTs;
    void handleClueResponse(const std::string& response);
};
