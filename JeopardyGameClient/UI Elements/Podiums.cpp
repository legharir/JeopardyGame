#include "Podiums.h"

Podiums::Podiums()
{
    if (!m_font.loadFromFile("/System/Library/Fonts/SFNSMono.ttf"))
        throw;
}

void Podiums::setPlayerInfo(const PlayerInfo* playerInfo)
{
    m_podiums.clear();
    for (const auto& player : playerInfo->getPlayers())
    {
        Podium podium(player);
        podium.setFont(m_font);
        m_podiums.push_back(podium); 
    }
}

void Podiums::setPosition(float x, float y)
{
    auto xPos = x;
    const auto rightMargin = 100;
    for (auto& podium : m_podiums)
    {
        podium.setPosition(xPos, y);
        xPos += podium.getWidth() + rightMargin;
    }
}

void Podiums::setFont(const sf::Font& font)
{
    m_font = font;
}

void Podiums::highlightPodium(const Player* player)
{
    for (auto& podium : m_podiums)
    {
        if (player->getName() == podium.getPlayer()->getName())
            podium.setHighlighted(true);
    }
}

void Podiums::update()
{
    for (auto& podium : m_podiums)
        podium.update();
}

void Podiums::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& podium : m_podiums)
        target.draw(podium);
}
