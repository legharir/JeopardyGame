#include "IntroState.h"
#include "LobbyState.h"
#include "Utils.h"

#include "Button.h"

IntroState IntroState::m_introState;

void IntroState::init(Engine* game)
{
    std::cout << "[Client] In IntroState" << std::endl;
    m_game = game;

    sf::Font font;
    if (!font.loadFromFile("/System/Library/Fonts/SFNSMono.ttf"))
        throw;

    m_joinButton.setPosition(sf::Vector2f(300, 400));
    m_joinButton.setSize(sf::Vector2f(150, 40));
    m_joinButton.setFont(font);
    m_joinButton.setText("Join Game");

    m_nameField.setLabel("Name:");
    m_nameField.setPosition(sf::Vector2f(100, 100));
    m_nameField.setFont(font);
    m_nameField.setFocused(true);
}

void IntroState::handleEvent(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::TextEntered:
            handleTextEntered(event.text);
            break;
        case sf::Event::MouseButtonPressed:
            handleMouseButtonPressed(event.mouseButton);
            break;
    }
}

void IntroState::handleTextEntered(const sf::Event::TextEvent& text)
{
    m_nameField.onTextEntered(text.unicode);
}

void IntroState::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& mouse)
{
    // Check if the join game button was clicked.
    if (m_joinButton.clicked(mouse))
    {
        // Initialize the player and transition to the next state.
        m_game->initSelf(m_nameField.getText());
        changeState(m_game, LobbyState::getInstance());
    }
}

void IntroState::update()
{
}

void IntroState::draw(sf::RenderWindow& window)
{
    window.draw(m_nameField);
    window.draw(m_joinButton);
}
