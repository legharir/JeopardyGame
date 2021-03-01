#pragma once

#include "GameMessage.h"
#include "GameState.h"

#include "Buzzer.h"
#include "Clue.h"
#include "Podiums.h"

#include <SFML/Graphics.hpp>

class BuzzInState : public GameState
{

public:
    void init(Engine* game);

    void handleEvent(const sf::Event& event);

    void handleBuzzIn(const BuzzInMessage& message);
    void handleSubRoundEnded(const SubRoundEndedMessage& message);
    void handleCluePickingStarted(const CluePickingStartedMessage& message);

    void update();
    void draw(sf::RenderWindow& window);

    void setClue(const Clue& clue);
    void setBuzzInDeadline(long long deadline);

    // Singleton class shouldn't be copyable or deletable.
    BuzzInState(BuzzInState& other) = delete;
    void operator=(const BuzzInState&) = delete;

    static BuzzInState* getInstance()
    {
        return &m_buzzInState;
    }

protected:
    BuzzInState()
    {
    }

private:
    Engine* m_game;
    static BuzzInState m_buzzInState;
    sf::Font m_font;

    // Event handlers.
    void handleTextEntered(const sf::Event::TextEvent& text);

    // UI elements.
    Clue m_clue;
    Buzzer m_buzzer;
    Podiums m_podiums;
};
