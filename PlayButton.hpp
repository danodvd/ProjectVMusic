#pragma once
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/ConvexShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class PlayButton {
public:
    PlayButton(sf::Vector2f pos);

    void Draw(sf::RenderTarget& target, bool isPlaying) const;

    bool Contains(sf::Vector2f mousePos) const;

    void SetPosition(sf::Vector2f newPos);
private:
    sf::FloatRect bounds;

    sf::CircleShape circle;
    sf::ConvexShape pauseTriangle;
    sf::RectangleShape bar1, bar2;

    static constexpr float TRIANGLE_X = 18;
    static constexpr float UPPER_BOUND = 12;
    static constexpr float TRIANGLE_LOWER_BOUND = 38;
    static constexpr float TRIANGLE_MID_BOUND = 25;
};