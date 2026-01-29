#include "ButtonNav.hpp"

static sf::FloatRect UnionRect(const sf::FloatRect& a, const sf::FloatRect& b) {
    float left = std::min(a.left, b.left);
    float top = std::min(a.top, b.top);
    float right = std::max(a.left + a.width, b.left + b.width);
    float bottom = std::max(a.top + a.height, b.top + b.height);
    return sf::FloatRect(left, top, right - left, bottom - top);
}

NavButton::NavButton(const sf::Vector2f& pos, Direction dir) {
    SetPosition(pos, dir);
}

void NavButton::SetPosition(sf::Vector2f centerPos, Direction dir) {
    float size = 20.f;        
    float height = size * 2;    
    float arrowW = size + 5;  
    float barW = 4.f;          
    float spacing = 0.f;      

    float totalWidth = arrowW + spacing + barW; 

    float startX = centerPos.x - (totalWidth / 2.f);
    float startY = centerPos.y - (height / 2.f);

    arrow.setPointCount(3);
    arrow.setFillColor(sf::Color::White);

    if (dir == Direction::Next) {
        arrow.setPoint(0, { 0, 0 });
        arrow.setPoint(1, { 0, height });
        arrow.setPoint(2, { arrowW, height / 2.f }); // Punta
        arrow.setPosition(startX, startY);

        bar.setSize({ barW, height });
        bar.setPosition(startX + arrowW + spacing, startY);
    }
    else {

        bar.setSize({ barW, height });
        bar.setPosition(startX, startY);

        arrow.setPoint(0, { arrowW, 0 });
        arrow.setPoint(1, { arrowW, height });
        arrow.setPoint(2, { 0, height / 2.f }); 
        arrow.setPosition(startX + barW + spacing, startY);
    }

    bar.setFillColor(sf::Color::White);
    bounds = sf::FloatRect(startX - 5, startY - 5, totalWidth + 10, height + 10);
}

void NavButton::Draw(sf::RenderTarget& target) const {
    target.draw(arrow);
    target.draw(bar);
}

bool NavButton::Contains(const sf::Vector2f& mouse) const {
    return bounds.contains(mouse);
}
