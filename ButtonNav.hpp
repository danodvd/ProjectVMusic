#pragma once
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/ConvexShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class NavButton {
public:
    enum class Direction {
        Next,
        Previous
    };

    NavButton(const sf::Vector2f& pos, Direction dir);

    void Draw(sf::RenderTarget& target) const;
    bool Contains(const sf::Vector2f& mouse) const;

    void SetPosition(sf::Vector2f newPos, Direction dir);
private:
    sf::ConvexShape arrow;
    sf::RectangleShape bar;
    sf::FloatRect bounds;

    static constexpr float HEIGHT = 20.f;
    static constexpr float ARROW_WIDTH = 15.f;
    static constexpr float BAR_WIDTH = 3.f;
};