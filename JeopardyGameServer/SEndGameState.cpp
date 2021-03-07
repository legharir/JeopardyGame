#include "SEndGameState.h"

SEndGameState SEndGameState::m_sEndGameState;

void SEndGameState::init(Game* game)
{
    m_game = game;
    m_game->onFinalJeopardyEnd();
}
