#include "Podium.h"

#include "Utils.h"

#include <algorithm>

Podium::Podium(const Player* player)
    : m_player(player)
{
}

const Player* Podium::getPlayer() const
{
    return m_player;
}

float Podium::getWidth() const
{
    const auto nameWidth = m_name.getLocalBounds().width;
    const auto balanceWidth = m_balance.getLocalBounds().width;
    return std::max(nameWidth, balanceWidth);
}

void Podium::setPosition(float x, float y)
{
    m_name.setPosition(x, y);
    m_balance.setPosition(x, y + m_name.getCharacterSize());
}

void Podium::setFont(const sf::Font& font)
{
    m_name.setFont(font);
    m_balance.setFont(font);
}

void Podium::setHighlighted(bool highlight)
{
    const auto color = highlight ? sf::Color(0, 191, 255) : sf::Color::White;
	m_name.setFillColor(color);
	m_balance.setFillColor(color);
}

void Podium::update()
{
    m_name.setString(m_player->getName());
    const auto& balanceStr = Utils::intToDollarStr(m_player->getBalance());
    m_balance.setString(balanceStr);
}

void Podium::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_name);
    target.draw(m_balance);
}
