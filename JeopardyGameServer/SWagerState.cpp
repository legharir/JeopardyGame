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
    if (Utils::getMsSinceEpoch() > m_game->getWagerDeadline())
    {
        m_game->onFinalJeopardyPlayStart();
        m_game->changeState(SPlayState::getInstance());
    }
}

void SWagerState::handleWager(const WagerMessage& message, const Player& player)
{
    m_game->onWagerSubmitted(player.getName(), message.wager);
}
