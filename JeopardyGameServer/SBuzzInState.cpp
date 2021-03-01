#include "SBuzzInState.h"

#include "SPickState.h"
#include "SPlayState.h"
#include "Utils.h"

SBuzzInState SBuzzInState::m_buzzInState;

void SBuzzInState::init(Game* game)
{
    std::cout << "[Server] In BuzzInState" << std::endl;
    m_game = game;
    m_game->enableBuzzers();
}

void SBuzzInState::update()
{
    // End the subround if nobody buzzed in within the buzz in window.
    if (Utils::getMsSinceEpoch() > m_game->getBuzzInDeadline())
    {
        m_game->endSubRound();
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        changeState(m_game, SPickState::getInstance());
    }
}

void SBuzzInState::handleBuzzIn(const Player& player)
{
    // Ignore this player if they've already buzzed in previously for the clue.
    const auto& responded = m_game->getResponded();
    if (responded.count(player.getName()))
        return;

    m_game->onBuzzIn(player.getName());
    changeState(m_game, SPlayState::getInstance());
}
