#pragma once

#include "GameMessage.h"
#include "GameState.h"

#include "Clue.h"

#include <SFML/Graphics.hpp>

class ClueState : public GameState
{

public:
    void init(Engine* game);

    void handleEvent(const sf::Event& event);
    void handleBuzzerEnabled(const BuzzerEnabledMessage& message);

    void update();
    void draw(sf::RenderWindow& window);

    void setAnswer(const std::string& answer);

    // Singleton class shouldn't be copyable or deletable.
    ClueState(ClueState& other) = delete;
    void operator=(const ClueState&) = delete;

    static ClueState* getInstance()
    {
        return &m_clueState;
    }

protected:
    ClueState()
    {
    }

private:
    Engine* m_game;
    static ClueState m_clueState;
    sf::Font m_font;

    // UI elements.
    Clue m_clue;
};
