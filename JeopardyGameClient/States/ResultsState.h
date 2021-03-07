#pragma once

#include "GameState.h"

#include <SFML/Graphics.hpp>

class ResultsState : public GameState
{

public:
    void init(Engine* game);

    void handleEvent(const sf::Event& event);
    
    void handleFinalJeopardyResults(const FinalJeopardyResultsMessage& message);

    void update();
    void draw(sf::RenderWindow& window);

    // Singleton class shouldn't be copyable or deletable.
    ResultsState(ResultsState& other) = delete;
    void operator=(const ResultsState&) = delete;

    static ResultsState* getInstance()
    {
        return &m_resultsState;
    }

protected:
    ResultsState()
    {
    }

private:
    static ResultsState m_resultsState;
    Engine* m_game;

    std::string m_correctResponse;
    std::vector<FinalJeopardyResult> m_finalJeopardyResults;
    unsigned int m_curResultInd = 0;
    
    // The time that has been elapsed displaying the current result.
    sf::Clock m_clock;

    // UI elements.
    sf::Font m_font;
    sf::Text m_curPlayerName;
    sf::Text m_curPlayerBalance;
    sf::Text m_curPlayerResponse;
    sf::Text m_curPlayerWager;
    sf::Text m_question;
    
    void initializeCurResult();
};

