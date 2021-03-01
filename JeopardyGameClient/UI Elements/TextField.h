// A basic controlled text field component.

#pragma once

#include <SFML/Graphics.hpp>

class TextField : public sf::Drawable
{
public:
    TextField(const sf::Vector2f& pos = sf::Vector2f(0, 0), const std::string& label = "",
              const sf::Font& font = sf::Font(), const sf::Vector2f& size = sf::Vector2f(300, 50),
              const sf::Color& fieldOutlineColor = sf::Color(), const sf::Color& textColor = sf::Color::Black);

    std::string getText() const;
    bool isFocused() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;

    void setPosition(const sf::Vector2f& pos);
    void setFont(const sf::Font& font);
    void setLabel(const std::string& label);
    void setFocused(bool focused);
    void setOutlineColor(const sf::Color& color);

    // Returns true and focuses itself if clicked, other unfocuses itself and returns false.
    bool onClick(const sf::Event::MouseButtonEvent& mouse);
    void onTextEntered(sf::Uint32 character);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::Vector2f m_pos;
    std::string m_label;
    sf::Font m_font;
    sf::Vector2f m_size;
    sf::Color m_fieldOutlineColor;
    sf::Color m_textColor;
    std::string m_text;
    bool m_focused;
};
