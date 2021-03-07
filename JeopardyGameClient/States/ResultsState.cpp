#include "ResultsState.h"

#include "Utils.H"
#include "ResourcePath.hpp"

ResultsState ResultsState::m_resultsState;

void ResultsState::init(Engine* game)
{
    std::cout << "[Client] In ResultsState" << std::endl;
    
    m_game = game;
    m_curResultInd = 0;
    
    if (!m_font.loadFromFile(resourcePath() + "KORIN.ttf"))
        throw;

    m_curPlayerName = sf::Text("", m_font, 30);
    m_curPlayerName.setPosition(400, 100);
    m_curPlayerName.setFillColor(sf::Color::White);

    m_curPlayerBalance = sf::Text("", m_font, 30);
    m_curPlayerBalance.setPosition(400, 150);
    m_curPlayerName.setFillColor(sf::Color::White);
    
    m_curPlayerResponse = sf::Text("", m_font, 30);
    m_curPlayerResponse.setPosition(400, 200);
    m_curPlayerResponse.setFillColor(sf::Color::Black);
    
    m_curPlayerWager = sf::Text("", m_font, 30);
    m_curPlayerWager.setPosition(400, 250);
    m_curPlayerWager.setFillColor(sf::Color::Black);
}

void ResultsState::handleFinalJeopardyResults(const FinalJeopardyResultsMessage& message)
{
    m_correctResponse = message.question;
    m_finalJeopardyResults = message.results;
    initializeCurResult();
    m_clock.restart();
}

void ResultsState::handleEvent(const sf::Event& event)
{
}

void ResultsState::update()
{
    if (m_finalJeopardyResults.size() == 0) return;
    if (m_curResultInd >= m_finalJeopardyResults.size())
    {
        // Transition to next state here instead of returning.
        return;
    }

    const auto resultToDisplay = m_finalJeopardyResults.at(m_curResultInd);
    const auto elapsedSeconds = m_clock.getElapsedTime().asSeconds();
    
    if (elapsedSeconds > 15)
    {
        ++m_curResultInd;
        if (m_curResultInd >= m_finalJeopardyResults.size()) return;
        initializeCurResult();
        m_clock.restart();
    }
    // As the time elapses, display the player's response then wager, for dramatic effect.
    else if (elapsedSeconds > 11)
        m_curPlayerBalance.setString(Utils::intToDollarStr(resultToDisplay.finalBalance));
    else if (elapsedSeconds > 9)
        m_curPlayerWager.setFillColor(sf::Color::White);
    else if (elapsedSeconds > 7)
        m_curPlayerResponse.setFillColor(resultToDisplay.correct ? sf::Color::Green : sf::Color::Red);
    else if (elapsedSeconds > 4)
        m_curPlayerResponse.setFillColor(sf::Color::White);
    else if (elapsedSeconds > 2)
        m_curPlayerResponse.setFillColor(sf::Color::White);
}

void ResultsState::draw(sf::RenderWindow& window)
{
    window.draw(m_curPlayerName);
    window.draw(m_curPlayerBalance);
    window.draw(m_curPlayerResponse);
    window.draw(m_curPlayerWager);
}

void ResultsState::initializeCurResult()
{
    const auto resultToDisplay = m_finalJeopardyResults.at(m_curResultInd);
    const auto balanceBefore = m_game->getPlayerInfo()->getPlayerBalance(resultToDisplay.playerName);
    
    m_curPlayerName.setString(resultToDisplay.playerName);
    m_curPlayerBalance.setString(Utils::intToDollarStr(balanceBefore));

    // Fill black as a hacky way to set them to "hidden".
    m_curPlayerResponse.setString("Response: " + resultToDisplay.response);
    m_curPlayerResponse.setFillColor(sf::Color::Black);
    
    m_curPlayerWager.setString("Wager: " + Utils::intToDollarStr(resultToDisplay.wager));
    m_curPlayerWager.setFillColor(sf::Color::Black);
}
