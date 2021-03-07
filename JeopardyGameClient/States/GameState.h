#pragma once


#include "Engine.h"
#include "GameMessage.h"

#include <SFML/Graphics.hpp>
#include <iostream>

class GameState
{
public:
    virtual void init(Engine* game) = 0;

    virtual void handleEvent(const sf::Event& event) = 0;

    virtual void handlePlayerJoined(const PlayerJoinedMessage& message){};
    virtual void handleGameStart(const GameStartMessage& message){};
    virtual void handleRoundStart(const RoundStartMessage& message){};
    virtual void handleCluePickingStarted(const CluePickingStartedMessage& message){};
    virtual void handleCluePicked(const CluePickedMessage& message){};
    virtual void handleWagerReceived(const WagerMessage& message){};
    virtual void handleDailyDouble(const DailyDoubleMessage& message){};
    virtual void handleBuzzerEnabled(const BuzzerEnabledMessage& message){};
    virtual void handleBuzzIn(const BuzzInMessage& message){};
    virtual void handlePartialClueResponse(const PartialClueResponseMessage& message){};
    virtual void handleResponseReceived(const ResponseReceivedMessage& message){};
    virtual void handleSubRoundEnded(const SubRoundEndedMessage& message){};
    virtual void handleFinalJeopardy(const FinalJeopardyMessage& message){};
    virtual void handleFinalJeopardyEnd(const FinalJeopardyEndMessage& message){};
    virtual void handleFinalJeopardyResults(const FinalJeopardyResultsMessage& message){};

    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void update() = 0;

    void changeState(Engine* game, GameState* state)
    {
        game->changeState(state);
    }

protected:
    GameState()
    {
    }
};
