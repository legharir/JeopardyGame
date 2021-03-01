#pragma once

#include "Podium.h"
#include "PlayerInfo.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Podiums : public sf::Drawable
{
public:
    Podiums();

    void setPlayerInfo(const PlayerInfo* playerInfo);
    void setPosition(float x, float y);
    void setFont(const sf::Font& font);
    void highlightPodium(const Player* player);

    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::Font m_font;
    std::vector<Podium> m_podiums;
};
