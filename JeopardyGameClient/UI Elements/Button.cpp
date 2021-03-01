#include "Button.h"

#include "Utils.h"

#include <iostream>

Button::Button(const sf::Vector2f& pos, const std::string& buttonText, const sf::Font& font, const sf::Vector2f size,
               const sf::Color& color, const sf::Color& textColor)
    : m_pos(pos)
    , m_buttonText(buttonText)
    , m_font(font)
    , m_size(size)
    , m_color(color)
    , m_textColor(textColor)
    , m_charSize(16)
{
}

sf::Vector2f Button::getPosition() const
{
    return m_pos;
}

std::string Button::getText() const
{
    return m_buttonText;
}

void Button::setPosition(const sf::Vector2f& pos)
{
    m_pos = pos;
}

sf::Vector2f Button::getSize() const
{
    return m_size;
}

void Button::setText(const std::string& buttonText)
{
    m_buttonText = buttonText;
}

void Button::setSize(const sf::Vector2f& size)
{
    m_size = size;
}

void Button::setFont(const sf::Font& font)
{
    m_font = font;
}

void Button::setCharacterSize(unsigned charSize)
{
    m_charSize = charSize;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // The button.
    sf::RectangleShape button(m_size);
    button.setPosition(m_pos);
    button.setFillColor(m_color);
    target.draw(button);

    // The button text.
    auto wrappedText = getWrappedText(m_buttonText, m_size.x);
    centerText(wrappedText);
    for (const auto& line : wrappedText)
        target.draw(line);
}

std::vector<sf::Text> Button::getWrappedText(const std::string& text, float maxLineLength) const
{
    std::vector<sf::Text> wrappedText;
    std::string curLine;

    for (const auto& word : Utils::split(text, " "))
    {
        const auto& part = curLine.empty() ? word : " " + word;
        sf::Text line(curLine + part, m_font, m_charSize);
        const auto& newLength = line.getLocalBounds().width;

        if (newLength < maxLineLength)
        {
            curLine += part;
        }
        else
        {
            wrappedText.push_back(sf::Text(curLine, m_font, m_charSize));
            curLine = part;
        }
    }
    wrappedText.push_back(sf::Text(curLine, m_font, m_charSize));

    return wrappedText;
}

void Button::centerText(std::vector<sf::Text>& text) const
{
    if (text.empty())
        return;

    const auto lineHeight = text[0].getCharacterSize();
    const auto totalHeight = text.size() * lineHeight;
    const auto top = (m_size.y - totalHeight) / 2;

    for (unsigned i = 0; i < text.size(); ++i)
    {
        auto& line = text[i];
        const auto& lineWidth = line.getLocalBounds().width;

        sf::Vector2f offset;
        offset.x = (m_size.x - lineWidth) / 2;
        offset.y = top + lineHeight * i;

        line.setPosition(m_pos + offset);
        line.setFillColor(m_textColor);
    }
}

bool Button::clicked(const sf::Event::MouseButtonEvent& mouse) const
{
    return areCoordsInside(mouse.x, mouse.y);
}

bool Button::areCoordsInside(int x, int y) const
{
    bool insideX = x > m_pos.x && x < m_pos.x + m_size.x;
    bool insideY = y > m_pos.y && y < m_pos.y + m_size.y;
    return insideX && insideY;
}
