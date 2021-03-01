#include "SWagerState.h"

#include "SBuzzInState.h"
#include "SPlayState.h"
#include "Utils.h"
#include <iostream>

SWagerState SWagerState::m_sWagerState;

void SWagerState::init(Game* game)
{
    std::cout << "[Server] In WagerState" << std::endl;
    m_game = game;
}

void SWagerState::update()
{
}

void SWagerState::handleWager(const WagerMessage& message, const Player& player)
{
    // If we're not in final Jeopardy!, ignore this message if it wasn't sent by
    // the clue picker.
    const auto isFinalJeopardyRound = m_game->getCurRound() != Round::FINAL_JEOPARDY;
    if (isFinalJeopardyRound && player.getName() != m_game->getPickerName())
        return;

    if (!isFinalJeopardyRound)
    {
        m_game->onWagerSubmitted(player.getName(), message.wager);
        changeState(m_game, SPlayState::getInstance());
    }
}
