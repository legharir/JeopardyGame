#include "SIntroState.h"

#include "DatasetReader.h"
#include "SPickState.h"

SIntroState SIntroState::m_sIntroState;

void SIntroState::init(Game* game)
{
    m_game = game;
}

void SIntroState::handlePlayerJoined(const PlayerJoinedMessage& message, sf::TcpSocket* socket)
{
    m_game->addPlayer(socket, message.playerName);
}

void SIntroState::handleGameStart(const GameStartMessage& message)
{
    m_game->startGame(message.season, message.episode);
    // TODO: reverse these? currently the client shows blank if I do this since
    // roundStartMessages are processed in PickState instead of LobbyState for
    // some reason.
    m_game->startCluePickingPhase();
    m_game->startRound(Round::JEOPARDY);
    changeState(m_game, SPickState::getInstance());
}
