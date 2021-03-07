#include "PlayState.h"

#include "BuzzInState.h"
#include "PickState.h"
#include "ResultsState.h"

#include "Utils.H"

#include <algorithm>

PlayState PlayState::m_playState;

void PlayState::init(Engine* game)
{
    std::cout << "[Client] In PlayState" << std::endl;
    m_game = game;
    m_responseReceived = false;
    m_isResponderSelf = m_game->getRound() == Round::FINAL_JEOPARDY ?
        true : m_game->getClueResponder() == m_game->getSelf();

    if (!m_font.loadFromFile("/System/Library/Fonts/SFNSMono.ttf"))
        throw;

    const auto windowSize = m_game->getWindowSize();
    
    const auto clueSize = sf::Vector2f(1000, 400);
    m_clue.setSize(clueSize);
    m_clue.setCharacterSize(30);
    m_clue.setPosition(sf::Vector2f((windowSize.x - clueSize.x) / 2, 20));
    
    m_deadlineBar = DeadlineBar(1000);
    m_deadlineBar.setPosition(sf::Vector2f((windowSize.x - m_deadlineBar.getWidth()) / 2, 450));
    m_deadlineBar.setDeadline(m_responseDeadline);

    m_responseField = TextField();
    m_responseField.setPosition(sf::Vector2f(500, 550));
    m_responseField.setLabel("What/Who is...");
    m_responseField.setFont(m_font);
    m_responseField.setFocused(m_isResponderSelf);
    
    const auto& bottom = windowSize.y;
    m_podiums.setPlayerInfo(m_game->getPlayerInfo());
    // 30 is font size, 10 is margin.
    m_podiums.setPosition(0, bottom - 30 * 2 - 10);
    m_podiums.highlightPodium(m_game->getClueResponder());
}

void PlayState::handleEvent(const sf::Event& event)
{
    // Only the responder is allowed to type in the response to the clue.
    if (!m_isResponderSelf)
        return;

    switch (event.type)
    {
        case sf::Event::TextEntered:
            handleTextEntered(event.text);
            break;
        case sf::Event::KeyPressed:
            handleKeyPressed(event.key);
            break;
        default:
            break;
    }
}

void PlayState::handlePartialClueResponse(const PartialClueResponseMessage& message)
{
    m_responseField.onTextEntered(message.character);
}

void PlayState::handleResponseReceived(const ResponseReceivedMessage& message)
{
    // Update the player balance.
    m_game->updatePlayerBalance(message.responder, message.balanceDelta);

    // Highlight the response field green/red to indicate whether
    // the response was correct or not.
    if (message.responseAccepted)
        m_responseField.setOutlineColor(sf::Color::Green);
    else
        m_responseField.setOutlineColor(sf::Color::Red);

    m_responseReceived = true;
}

void PlayState::handleSubRoundEnded(const SubRoundEndedMessage& message)
{
    m_clue.setQuestion(message.question);
}

void PlayState::handleCluePickingStarted(const CluePickingStartedMessage& message)
{
    m_game->setCluePicker(message.picker);
    changeState(m_game, PickState::getInstance());
}

void PlayState::handleBuzzerEnabled(const BuzzerEnabledMessage& message)
{
    const auto& buzzInState = BuzzInState::getInstance();
    buzzInState->setBuzzInDeadline(message.buzzInDeadline);
    changeState(m_game, BuzzInState::getInstance());
}

void PlayState::update()
{
    m_podiums.update();
    if (!m_responseReceived)
        m_deadlineBar.update();
}

void PlayState::draw(sf::RenderWindow& window)
{
    window.draw(m_responseField);
    window.draw(m_clue);
    window.draw(m_deadlineBar);
    window.draw(m_podiums);
}

void PlayState::setClue(const Clue& clue)
{
    m_clue = clue;
}

void PlayState::setResponseDeadline(long long responseDeadline)
{
    m_responseDeadline = responseDeadline;
}

void PlayState::handleTextEntered(const sf::Event::TextEvent& text)
{
    if (m_game->getRound() == Round::FINAL_JEOPARDY)
    {
        m_responseField.onTextEntered(text.unicode);
        return;
    }

    // Send the partial response to the server so that other players
    // may see the response as it's being typed.
    PartialClueResponseMessage partialClueResponseMessage(text.unicode);
    sf::Packet packet;
    packet << partialClueResponseMessage;
    m_game->getClient()->send(packet);
}

void PlayState::handleKeyPressed(const sf::Event::KeyEvent& key)
{
    sf::Packet packet;
    if (key.code == sf::Keyboard::Enter)
    {
        // Send the response to the server if the player hit enter.
        ClueResponseMessage clueResponseMessage(m_responseField.getText());
        packet << clueResponseMessage;
        m_game->getClient()->send(packet);
    }
}

void PlayState::handleFinalJeopardyEnd(const FinalJeopardyEndMessage& message)
{
    m_game->changeState(ResultsState::getInstance());
}
