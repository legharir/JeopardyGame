#include "SPlayState.h"

#include "SBuzzInState.h"
#include "SPickState.h"
#include "Utils.h"

SPlayState SPlayState::m_sPlayState;

void SPlayState::init(Game* game)
{
    std::cout << "[Server] In PlayState" << std::endl;
    m_game = game;
    m_responseDeadlineExceededTs = 0;
}

void SPlayState::update()
{
    auto reponseDeadlineExceeded = Utils::getMsSinceEpoch() > m_game->getResponseDeadline();
    if (reponseDeadlineExceeded)
        handleClueResponse("");
}

void SPlayState::handlePartialClueResponse(const PartialClueResponseMessage& message, const Player& player)
{
    m_game->onPartialClueResponse(player.getName(), message.character);
}

void SPlayState::handleClueResponse(const ClueResponseMessage& message, const Player& player)
{
    // Ignore this message if it wasn't the clue responder who sent it.
    if (player.getName() != m_game->getResponderName())
        return;

    handleClueResponse(message.response);
}

void SPlayState::handleClueResponse(const std::string& response)
{
    const auto& correctResponse = m_game->getCurClue().getQuestion();
    const auto isResponseCorrect = Utils::contains(correctResponse, response);

	m_game->onClueResponse(isResponseCorrect); 

    const auto subRoundOver =
            isResponseCorrect || m_game->allPlayersResponded() || m_game->getCurClue().isDailyDouble();
    if (subRoundOver)
	{
        m_game->endSubRound();
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        changeState(m_game, SPickState::getInstance());
	}
    else
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        changeState(m_game, SBuzzInState::getInstance());
	}

}
