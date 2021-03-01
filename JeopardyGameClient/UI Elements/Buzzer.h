#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Buzzer : public sf::Drawable
{
public:
    Buzzer();

    void setFont(const sf::Font& font);
    void setHidden(bool hidden);
    // Sets the buzzer to time out `deadline` time units from now.
    void setTimer(sf::Time timeToDeadline);
    void setPosition(const sf::Vector2f& pos);
    void setMaxSize(const sf::Vector2f& pos);

    void update(sf::Time deltaTime);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    // The deadline to buzz in, in ms since epoch.
    bool m_started;

    sf::Time m_timeToDeadline;
    float m_widthPerUnitTime;
    float m_maxWidth;
    bool m_hidden;
    sf::Vector2f m_pos;
    sf::Font m_font;
    sf::Vector2f m_size;
    sf::RectangleShape m_buzzer;
};
