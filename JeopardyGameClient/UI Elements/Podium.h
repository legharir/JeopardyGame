#pragma once

#include "Player.h"

#include <SFML/Graphics.hpp>

class Podium : public sf::Drawable
{
public:
    Podium(const Player* player);

    const Player* getPlayer() const;
    float getWidth() const;

    void setFont(const sf::Font& font);
    void setPosition(float x, float y);
    void setHighlighted(bool highlight);

    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    const Player* m_player;
    sf::Text m_name;
    sf::Text m_balance;
};
