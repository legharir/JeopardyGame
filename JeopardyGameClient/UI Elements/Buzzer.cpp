#include "Buzzer.h"

#include "Utils.h"
#include <iostream>

Buzzer::Buzzer()
    : m_started(false)
    , m_hidden(false)
    , m_size(0, 50)
    , m_maxWidth(1000)
    , m_widthPerUnitTime(0)
{
    m_buzzer.setFillColor(sf::Color::Green);
}

void Buzzer::setFont(const sf::Font& font)
{
    m_font = font;
}

void Buzzer::setHidden(bool hidden)
{
    m_hidden = hidden;
}

void Buzzer::setTimer(sf::Time timeToDeadline)
{
    m_timeToDeadline = timeToDeadline;
    m_widthPerUnitTime = m_maxWidth / timeToDeadline.asMilliseconds();
}

void Buzzer::setPosition(const sf::Vector2f& pos)
{
    m_pos = pos;
}

void Buzzer::setMaxSize(const sf::Vector2f& size)
{
    m_size = size;
    m_maxWidth = size.x;
}

void Buzzer::update(sf::Time deltaTime)
{
    if (m_timeToDeadline.asMilliseconds() < 0)
        return;

    // The size of the buzzer should be proprotional to the amount of time left
    // to buzz in.
    m_timeToDeadline -= deltaTime;
    m_size.x = m_timeToDeadline.asMilliseconds() * m_widthPerUnitTime;

    // Keep the buzzer centered to create the effect of it shrinking from both
    // sides instead of just one side.
    const auto centerOffset = (m_maxWidth - m_size.x) / 2;
    m_buzzer.setPosition(m_pos.x + centerOffset, m_pos.y);
    m_buzzer.setSize(m_size);
}

void Buzzer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!m_hidden)
        target.draw(m_buzzer);
}
