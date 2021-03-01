#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

class Button : public sf::Drawable
{
public:
    // TODO: refactor this constructor to only take a few (necessary?) params
    // TODO: initialize the font here so that it doesn't get replicated a million times.
    Button(const sf::Vector2f& pos = sf::Vector2f(0, 0), const std::string& buttonText = "",
           const sf::Font& font = sf::Font(), const sf::Vector2f size = sf::Vector2f(160, 80),
           const sf::Color& color = sf::Color::Blue, const sf::Color& textColor = sf::Color::White);

    sf::Vector2f getSize() const;
    sf::Vector2f getPosition() const;
    std::string getText() const;

    void setText(const std::string& buttonText);
    void setSize(const sf::Vector2f& size);
    void setPosition(const sf::Vector2f& pos);
    void setFont(const sf::Font& font);
    void setCharacterSize(unsigned charSize);

    bool areCoordsInside(int x, int y) const;
    bool clicked(const sf::Event::MouseButtonEvent& mouse) const;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    std::vector<sf::Text> getWrappedText(const std::string& text, float maxLineLength) const;
    void centerText(std::vector<sf::Text>& text) const;

    // TODO: can only the sf::RectangleShape and sf::Text shape be stored? It's
    // kind of ugly to store all these UI related members when SFML can do it
    // for us.
    sf::Vector2f m_size;
    sf::Vector2f m_pos;
    sf::Color m_color;
    std::string m_buttonText;
    sf::Color m_textColor;
    unsigned m_charSize;
    sf::Font m_font;
    std::vector<sf::Text> m_wrappedButtonText;
};
