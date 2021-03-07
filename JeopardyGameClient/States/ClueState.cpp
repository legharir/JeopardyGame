#include "ClueState.h"

#include "BuzzInState.h"
#include "Utils.H"
#include "ResourcePath.hpp"

ClueState ClueState::m_clueState;

void ClueState::init(Engine* game)
{
    std::cout << "[Client] In CluState" << std::endl;
    m_game = game;

    if (!m_font.loadFromFile(resourcePath() + "KORIN.ttf"))
        throw;

    sf::Vector2f clueSize;
    clueSize.x = (float)m_game->getWindowSize().x;
    clueSize.y = (float)m_game->getWindowSize().y;
    m_clue.setSize(clueSize);
    m_clue.setCharacterSize(50);
    m_clue.setFont(m_font);
}

void ClueState::handleEvent(const sf::Event& event)
{
}

void ClueState::handleBuzzerEnabled(const BuzzerEnabledMessage& message)
{
    const auto& buzzInState = BuzzInState::getInstance();
    buzzInState->setClue(m_clue);
    buzzInState->setBuzzInDeadline(message.buzzInDeadline);

    changeState(m_game, buzzInState);
}

void ClueState::update()
{
}

void ClueState::draw(sf::RenderWindow& window)
{
    window.draw(m_clue);
}

void ClueState::setAnswer(const std::string& answer)
{
    m_clue.setText(answer);
}
