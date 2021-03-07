#pragma once

#include "GameState.h"

#include "Clue.h"
#include "TextField.h"
#include "Podiums.h"
#include "Buzzer.h"

class WagerState : public GameState
{

public:
    void init(Engine* game);

    void handleEvent(const sf::Event& event);
    void handleWagerReceived(const WagerMessage& message);

    void update();
    void draw(sf::RenderWindow& window);

    void setResponding(bool responding);
    void setCategory(const std::string& category);
    void setDeadline(long long deadline);
    
    // Singleton class shouldn't be copyable or deletable.
    WagerState(WagerState& other) = delete;
    void operator=(const WagerState&) = delete;

    static WagerState* getInstance()
    {
        return &m_wagerState;
    }

protected:
    WagerState()
    {
    }

private:
    static WagerState m_wagerState;
    Engine* m_game;

    bool m_responding;

    // UI elements.
    Clue m_clue;
    TextField m_wagerField;
    Podiums m_podiums;
    Buzzer m_buzzer;

    // Event handlers.
    void handleTextEntered(const sf::Event::TextEvent& text);
    void handleKeyPressed(const sf::Event::KeyEvent& key);
};
