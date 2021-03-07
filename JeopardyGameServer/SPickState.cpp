#include "SPickState.h"

#include "SBuzzInState.h"
#include "SWagerState.h"
#include "Utils.h"

#include <iostream>

SPickState SPickState::m_sPickState;

void SPickState::init(Game* game)
{
    std::cout << "[Server] In PickState" << std::endl;
    m_game = game;
    m_cluePickedTimestamp = 0;

    m_game->startCluePickingPhase();

    // If there are no more clues left in the current round, start the next round.
    // In the case that the current round is Final Jeopardy!, end the game.
    if (m_game->numCluesUnpicked() == 0)
    {
        if (m_game->getCurRound() == Round::JEOPARDY)
        {
            m_game->startRound(Round::DOUBLE_JEOPARDY);
        }
        else if (m_game->getCurRound() == Round::DOUBLE_JEOPARDY)
        {
            m_game->startRound(Round::FINAL_JEOPARDY);
            m_game->onFinalJeopardyStart();
            m_game->changeState(SWagerState::getInstance());
        }
        else
        {
            m_game->endGame();
        }
    }
}

void SPickState::update()
{
    auto clueWasPicked = m_cluePickedTimestamp > 0;
    auto buzzerDelayExceeded = Utils::getMsSinceEpoch() > m_cluePickedTimestamp + m_game->getBuzzerReadyDelay();
    if (clueWasPicked && buzzerDelayExceeded)
        changeState(m_game, SBuzzInState::getInstance());
}

void SPickState::handleCluePicked(const CluePickedMessage& message, const Player& player)
{
    const auto messageSentByPicker = m_game->getPickerName() == player.getName();
    const auto& clue = m_game->getClue(message.category, message.value);

    // Only process this message if it was sent by the picker and the chosen
    // clue wasn't already picked.
    if (!messageSentByPicker || clue.isPicked())
        return;

    if (clue.isDailyDouble())
    {
        m_game->onDailyDoublePicked(player.getName(), message.category, message.value);
        changeState(m_game, SWagerState::getInstance());
    }
    else
    {
        m_game->onCluePicked(message.category, message.value);
        m_cluePickedTimestamp = Utils::getMsSinceEpoch();
    }
}
