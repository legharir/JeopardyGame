#include "PickState.h"

#include "ClueState.h"
#include "WagerState.h"
#include "ResourcePath.hpp"

#include "Button.h"
#include "GameMessage.h"
#include "Podium.h"
#include "Utils.h"

#include <SFML/Network.hpp>
#include <algorithm>
#include <iostream>

PickState PickState::m_pickState;

void PickState::init(Engine* game)
{
    std::cout << "[Client] In PickState" << std::endl;
    m_game = game;

    if (!m_font.loadFromFile(resourcePath() + "KORIN.ttf"))
        throw;

    const auto& bottom = (float)m_game->getWindowSize().y;
    m_podiums.setPlayerInfo(m_game->getPlayerInfo());
    m_podiums.setPosition(0, bottom - 30 * 2 - 10);

    // Highlight the podium of the picker if one exists yet.
    if (m_game->getCluePicker())
        m_podiums.highlightPodium(m_game->getCluePicker());
}

void PickState::handleEvent(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::MouseButtonPressed:
            handleMouseButtonPressed(event.mouseButton);
            break;
    }
}

void PickState::handleRoundStart(const RoundStartMessage& message)
{
    m_playerBoard = message.playerBoard;
    m_game->setRound(message.round);
}

void PickState::handleCluePickingStarted(const CluePickingStartedMessage& message)
{
    m_game->setCluePicker(message.picker);
    // Highlight the clue picker's podium so the players know who's responsible
    // to pick the next clue.
    m_podiums.highlightPodium(m_game->getCluePicker());
}

void PickState::handleCluePicked(const CluePickedMessage& message)
{
    m_playerBoard.setPicked(message.category, message.value);
    const auto& clueState = ClueState::getInstance();
    clueState->setAnswer(message.answer);
    changeState(m_game, clueState);
}

void PickState::handleDailyDouble(const DailyDoubleMessage& message)
{
    m_playerBoard.setPicked(message.category, message.value);
    m_game->setClueResponder(m_game->getCluePicker()->getName());

    const auto& wagerState = WagerState::getInstance();
    wagerState->setCategory(message.category);
    wagerState->setResponding(m_game->getSelf() == m_game->getClueResponder());
    changeState(m_game, WagerState::getInstance());
}

void PickState::handleFinalJeopardy(const FinalJeopardyMessage& message)
{
    const auto& wagerState = WagerState::getInstance();
    wagerState->setCategory("Final Jeopardy: " + message.category);
    wagerState->setResponding(true);
    wagerState->setDeadline(message.wagerDeadline);
    changeState(m_game, WagerState::getInstance());
}

void PickState::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& mouse)
{
    // If a value cell was clicked, let the game server know to display the clue.
    if (m_playerBoard.clicked(mouse))
    {
        const auto& cell = m_playerBoard.getValueCellAtPos(mouse.x, mouse.y);
        if (!cell)
            return;

        CluePickedMessage message(cell->category, cell->value);
        sf::Packet packet;
        packet << message;
        m_game->getClient()->send(packet);
    }
}

void PickState::update()
{
    m_podiums.update();
}

void PickState::draw(sf::RenderWindow& window)
{
    window.draw(m_playerBoard);
    window.draw(m_podiums);
}
