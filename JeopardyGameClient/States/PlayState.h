#pragma once

#include "GameMessage.h"
#include "GameState.h"

#include "Clue.h"
#include "TextField.h"
#include "Podiums.h"
#include "DeadlineBar.hpp"

#include <SFML/Graphics.hpp>

class PlayState : public GameState
{

public:
    void init(Engine* game);

    void handleEvent(const sf::Event& event);

    void handlePartialClueResponse(const PartialClueResponseMessage& message);
    void handleResponseReceived(const ResponseReceivedMessage& message);
    void handleSubRoundEnded(const SubRoundEndedMessage& message);
    void handleCluePickingStarted(const CluePickingStartedMessage& message);
    void handleBuzzerEnabled(const BuzzerEnabledMessage& message);
    void handleFinalJeopardyEnd(const FinalJeopardyEndMessage& message);

    void update();
    void draw(sf::RenderWindow& window);

    void setClue(const Clue& clue);
    void setResponseDeadline(long long responseDeadline);

    // Singleton class shouldn't be copyable or deletable.
    PlayState(PlayState& other) = delete;
    void operator=(const PlayState&) = delete;

    static PlayState* getInstance()
    {
        return &m_playState;
    }

protected:
    PlayState()
    {
    }

private:
    Engine* m_game;
    static PlayState m_playState;

    bool m_isResponderSelf;
    bool m_responseReceived;
    long long m_responseDeadline;

    // UI elements.
    sf::Font m_font;
    Clue m_clue;
    TextField m_responseField;
    DeadlineBar m_deadlineBar;
    Podiums m_podiums;

    // Event handlers.
    void handleTextEntered(const sf::Event::TextEvent& text);
    void handleKeyPressed(const sf::Event::KeyEvent& key);
};
