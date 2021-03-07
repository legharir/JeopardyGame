//
//  DeadlineBar.cpp
//  JeopardyGameClient
//
//  Created by Rafay Leghari on 2021-02-28.
//  Copyright © 2021 Rafay Leghari. All rights reserved.
//

#include "DeadlineBar.hpp"

#include "Utils.H"
#include <iostream>

DeadlineBar::DeadlineBar(float width, unsigned int numRects) : m_width(width)
{
    
    float totalMarginWidth = (numRects - 1) * 10;
    m_rectWidth = (width - totalMarginWidth) / numRects;

    for (int i = 0u; i < numRects; ++i)
    {
        m_rects.push_back(sf::RectangleShape(sf::Vector2f(m_rectWidth, 20)));
    }
}

float DeadlineBar::getWidth() const
{
    auto width = 0;
    for (const auto& rect : m_rects)
        width += rect.getSize().x;
    return width;
}

void DeadlineBar::setPosition(sf::Vector2f pos)
{
    const auto margin = 10;
    for (auto& rect : m_rects)
    {
        rect.setPosition(pos);
        pos.x += margin + m_rectWidth;
    }
}

void DeadlineBar::setDeadline(long long deadline)
{
    // Reset the state of all the rectangles.
    for (auto& rect : m_rects)
    {
        rect.setOutlineColor(sf::Color::Green);
        rect.setFillColor(sf::Color::Red);
    }
    
    m_deadline = deadline;
    m_rectTimeRatio = (float)m_rects.size() / (deadline - Utils::getMsSinceEpoch());
}

void DeadlineBar::update()
{
    const auto timeToDeadline = m_deadline - Utils::getMsSinceEpoch();
    
    if (timeToDeadline < 0LL)
    {
        for (auto& rect : m_rects)
            rect.setFillColor(sf::Color(128, 128, 128));
        return;
    }

    const auto numLitRects = Utils::nearestOdd(timeToDeadline * m_rectTimeRatio);
    const auto numUnlitRects = (m_rects.size() - numLitRects);
    
    for (int i = 0u; i < numUnlitRects / 2; ++i)
    {
        m_rects.at(i).setFillColor(sf::Color(128, 128, 128));
        m_rects.at(m_rects.size() - i - 1).setFillColor(sf::Color(128, 128, 128));
    }
}

void DeadlineBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& rect : m_rects)
        target.draw(rect);
}
