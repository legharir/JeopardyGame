//
//  DeadlineBar.hpp
//  JeopardyGameClient
//
//  Created by Rafay Leghari on 2021-02-28.
//  Copyright © 2021 Rafay Leghari. All rights reserved.
//

#include <vector>
#include <SFML/Graphics.hpp>

class DeadlineBar : public sf::Drawable
{
public:
    DeadlineBar() = default;
    DeadlineBar(float width, unsigned int numRects = 9);
    
    float getWidth() const;
    
    void setDeadline(long long deadline);
    void setPosition(sf::Vector2f pos);

    void update();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    float m_width;
    float m_rectWidth;
    long long m_deadline;
    double m_rectTimeRatio;
    std::vector<sf::RectangleShape> m_rects;
};
