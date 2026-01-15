#pragma once
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "PlayBackInfo.hpp"

class ProgressBar {
public:
    ProgressBar(sf::Vector2f pos, sf::Vector2f size);

    void Update(const PlaybackInfo& info);

    void Draw(sf::RenderTarget& target) const;

    bool TrySeek(sf::Vector2f mousePos, double& outPct) const;

    void SetPosition(sf::Vector2f newPos);
    void SetSize(sf::Vector2f size);
private:
    sf::RectangleShape background, fill;
};