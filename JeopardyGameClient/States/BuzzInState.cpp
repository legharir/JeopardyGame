#include "BuzzInState.h"

#include "PickState.h"
#include "PlayState.h"
#include "Utils.H"

BuzzInState BuzzInState::m_buzzInState;

void BuzzInState::init(Engine* game)
{
    std::cout << "[Client] In BuzzInState" << std::endl;
    m_game = game;

    if (!m_font.loadFromFile("/System/Library/Fonts/SFNSMono.ttf"))
        throw;

    m_clue.setSize(sf::Vector2f(500, 500));
    m_clue.setCharacterSize(30);

    float windowWidth = (float)m_game->getWindowSize().x;
    m_buzzer.setMaxSize(sf::Vector2f(windowWidth, 50));
    m_buzzer.setPosition(sf::Vector2f(0, 500));
    m_buzzer.setFont(m_font);

    const auto& bottom = (float)m_game->getWindowSize().y;
    m_podiums.setPlayerInfo(m_game->getPlayerInfo());
    m_podiums.setPosition(0, bottom - 30 * 2);
}

void BuzzInState::handleEvent(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::TextEntered:
            handleTextEntered(event.text);
            break;
    }
}

void BuzzInState::handleBuzzIn(const BuzzInMessage& message)
{
    m_game->setClueResponder(message.playerName);

	const auto& playState = PlayState::getInstance();
	playState->setClue(m_clue);
	playState->setResponseDeadline(message.responseDeadline);
	changeState(m_game, playState);
}

void BuzzInState::handleSubRoundEnded(const SubRoundEndedMessage& message)
{
    // Reveal the correct question since the sub round is now over.
    m_clue.setQuestion(message.question);
}

void BuzzInState::handleCluePickingStarted(const CluePickingStartedMessage& message)
{
    m_game->setCluePicker(message.picker);
    changeState(m_game, PickState::getInstance());
}

void BuzzInState::update()
{
    m_buzzer.update(m_game->getDeltaTime());
}

void BuzzInState::draw(sf::RenderWindow& window)
{
    window.draw(m_clue);
    window.draw(m_buzzer);
    window.draw(m_podiums);
}

void BuzzInState::setClue(const Clue& clue)
{
    m_clue = clue;
}

void BuzzInState::setBuzzInDeadline(long long deadline)
{
    const auto now = Utils::getMsSinceEpoch();
    sf::Time timeToDeadline = sf::milliseconds((sf::Int32)deadline - (sf::Int32)now);
    m_buzzer.setTimer(timeToDeadline);
}

void BuzzInState::handleTextEntered(const sf::Event::TextEvent& text)
{
    if (text.unicode == ' ')
    {
        sf::Packet packet;
        packet << BuzzInMessage();
        m_game->getClient()->send(packet);
    }
}
