#include "PlayButton.hpp"

PlayButton::PlayButton(sf::Vector2f pos) {
    circle.setRadius(25);
    circle.setOrigin(25, 25);
    circle.setPosition(pos);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(2);
    circle.setOutlineColor(sf::Color::White);

    pauseTriangle.setPointCount(3);
    pauseTriangle.setPoint(0, { pos.x - 5, pos.y - 10 });
    pauseTriangle.setPoint(1, { pos.x - 5, pos.y + 10 });
    pauseTriangle.setPoint(2, { pos.x + 12, pos.y }); 
    pauseTriangle.setFillColor(sf::Color::White);

    bar1.setSize({ 6, 20 }); bar1.setOrigin(3, 10); bar1.setPosition(pos.x - 6, pos.y);
    bar2.setSize({ 6, 20 }); bar2.setOrigin(3, 10); bar2.setPosition(pos.x + 6, pos.y);
    bar1.setFillColor(sf::Color::White);
    bar2.setFillColor(sf::Color::White);

    bounds = circle.getGlobalBounds();
}

void PlayButton::Draw(sf::RenderTarget& target, bool isPlaying) const {
    target.draw(circle);

    if (isPlaying) {
        target.draw(bar1);
        target.draw(bar2);
    } else {
        target.draw(pauseTriangle);
    }
}

bool PlayButton::Contains(sf::Vector2f mousePos) const {
    return bounds.contains(mousePos);
}

void PlayButton::SetPosition(sf::Vector2f newPos) {
    circle.setPosition(newPos);

    pauseTriangle.setPoint(0, { newPos.x - 5, newPos.y - 10 });
    pauseTriangle.setPoint(1, { newPos.x - 5, newPos.y + 10 });
    pauseTriangle.setPoint(2, { newPos.x + 12, newPos.y });

    bar1.setPosition(newPos.x - 6, newPos.y);

    bar2.setPosition(newPos.x + 6, newPos.y);

    bounds = circle.getGlobalBounds();
}