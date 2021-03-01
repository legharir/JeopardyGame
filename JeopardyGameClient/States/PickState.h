#pragma once

#include "GameState.h"

#include "GameMessage.h"
#include "Podiums.h"

#include <SFML/Graphics.hpp>

class PickState : public GameState
{

public:
    void init(Engine* game);

    void handleEvent(const sf::Event& event);

    void handleRoundStart(const RoundStartMessage& message);
    void handleCluePickingStarted(const CluePickingStartedMessage& message);
    void handleCluePicked(const CluePickedMessage& message);
    void handleDailyDouble(const DailyDoubleMessage& message);
    void handleFinalJeopardy(const FinalJeopardyMessage& message);

    void update();
    void draw(sf::RenderWindow& window);

    // Single class shouldn't be copyable or deletable.
    PickState(PickState& other) = delete;
    void operator=(const PickState&) = delete;

    static PickState* getInstance()
    {
        return &m_pickState;
    }

protected:
    PickState()
    {
    }

private:
    static PickState m_pickState;
    Engine* m_game;

    // Event handlers.
    void handleMouseButtonPressed(const sf::Event::MouseButtonEvent& mouse);

    // UI Elements.
    DrawablePlayerBoard m_playerBoard;
    Podiums m_podiums;
    sf::Font m_font;
};
