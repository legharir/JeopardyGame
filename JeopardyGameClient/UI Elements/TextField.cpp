#include "TextField.h"

TextField::TextField(const sf::Vector2f& pos, const std::string& label, const sf::Font& font, const sf::Vector2f& size,
                     const sf::Color& fieldOutlineColor, const sf::Color& textColor)
    : m_pos(pos)
    , m_font(font)
    , m_size(size)
    , m_fieldOutlineColor(fieldOutlineColor)
    , m_textColor(textColor)
    , m_focused(false)
{
}

void TextField::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // The label.
    sf::Text label(m_label, m_font);
    label.setPosition(m_pos);

    // The text field.
    sf::RectangleShape textField(m_size);
    textField.setPosition(sf::Vector2f(m_pos.x, m_pos.y + m_size.y));
    if (m_focused)
    {
        textField.setOutlineColor(sf::Color(0, 191, 255));
        textField.setOutlineThickness(5.0);
    }
    if (m_fieldOutlineColor != sf::Color())
    {
        textField.setOutlineColor(m_fieldOutlineColor);
        textField.setOutlineThickness(5.0);
    }

    // The text.
    sf::Text text(m_text, m_font);
    text.setPosition(sf::Vector2f(m_pos.x, m_pos.y + m_size.y));
    text.setFillColor(m_textColor);

    target.draw(label);
    target.draw(textField);
    target.draw(text);
}

std::string TextField::getText() const
{
    return m_text;
}

bool TextField::isFocused() const
{
    return m_focused;
}

void TextField::setPosition(const sf::Vector2f& pos)
{
    m_pos = pos;
}

void TextField::setFont(const sf::Font& font)
{
    m_font = font;
}

void TextField::setLabel(const std::string& label)
{
    m_label = label;
}

void TextField::setFocused(bool focused)
{
    m_focused = focused;
}

void TextField::setOutlineColor(const sf::Color& color)
{
    m_fieldOutlineColor = color;
}

void TextField::onTextEntered(sf::Uint32 character)
{
    char c = character;
    if (c == '\b')
        m_text = m_text.substr(0, m_text.size() - 1);
    else
        m_text += c;
}

bool TextField::onClick(const sf::Event::MouseButtonEvent& mouse)
{
    bool insideX = mouse.x > m_pos.x && mouse.x < m_pos.x + m_size.x;
    // Multiply by 2 to account for the height of the text field.
    bool insideY = mouse.y > m_pos.y && mouse.y < m_pos.y + 2 * m_size.y;
    bool clicked = insideX && insideY;

    setFocused(clicked);
    return clicked;
}

sf::Vector2f TextField::getPosition() const
{
    return m_size;
}

sf::Vector2f TextField::getSize() const
{
    return m_pos;
}
