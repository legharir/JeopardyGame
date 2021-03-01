#include "WagerState.h"

#include "PlayState.h"

WagerState WagerState::m_wagerState;

void WagerState::init(Engine* game)
{
    std::cout << "[Client] In WagerState" << std::endl;
    m_game = game;

    sf::Font font;
    if (!font.loadFromFile("/System/Library/Fonts/SFNSMono.ttf"))
        throw;

    m_clue.setFont(font);
    m_clue.setSize(sf::Vector2f(500, 500));
    m_clue.setCharacterSize(30);

    m_wagerField = TextField();
    m_wagerField.setLabel("Wager ($):");
    m_wagerField.setPosition(sf::Vector2f(500, 550));
    m_wagerField.setFont(font);
    m_wagerField.setFocused(m_responding);

    const auto& bottom = (float)m_game->getWindowSize().y;
    m_podiums.setPlayerInfo(m_game->getPlayerInfo());
    // 30 is font size, 10 is margin.
    m_podiums.setPosition(0, bottom - 30 * 2 - 10);
    m_podiums.highlightPodium(m_game->getCluePicker());
}

void WagerState::handleEvent(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::TextEntered:
            handleTextEntered(event.text);
            break;
        case sf::Event::KeyPressed:
            handleKeyPressed(event.key);
            break;
    }
}

void WagerState::handleWagerReceived(const WagerMessage& message)
{
    m_clue.setAnswer(message.answer);

    const auto& playState = PlayState::getInstance();
    playState->setClue(m_clue);
    playState->setResponseDeadline(message.responseDeadline);
    changeState(m_game, PlayState::getInstance());
}

void WagerState::handleTextEntered(const sf::Event::TextEvent& text)
{
    if (!std::isdigit(text.unicode) && text.unicode != '\b')
        return;
	m_wagerField.onTextEntered(text.unicode);
}

void WagerState::handleKeyPressed(const sf::Event::KeyEvent& key)
{
    sf::Packet packet;
    if (key.code == sf::Keyboard::Enter)
    {
        // Send the wager to the server if the player hit enter.
        int wager = std::stoi(m_wagerField.getText());
        WagerMessage wagerMessage(wager);
        packet << wagerMessage;
        m_game->getClient()->send(packet);
    }
}

void WagerState::update()
{
    m_podiums.update();
}

void WagerState::draw(sf::RenderWindow& window)
{
    window.draw(m_clue);
    window.draw(m_wagerField);
    window.draw(m_podiums);
}

void WagerState::setResponding(bool responding)
{
    m_responding = responding;
}

void WagerState::setCategory(const std::string& category)
{
    m_clue.setAnswer(category);
}
