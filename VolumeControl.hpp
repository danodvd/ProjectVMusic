#pragma once
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/ConvexShape.hpp"
#include "UIUtils.hpp"
#include "VolumeIcons.hpp"
#include "VolumeChange.hpp"
#include <sstream>

class VolumeControl {
public:
    VolumeControl(sf::Vector2f pos);

    void LoadFont(std::string fontPath);

    void Update(float currentVolume);
    void Draw(sf::RenderTarget& target) const;

    float ComputeVolumeFromMouse(float mouseY) const;

    VolumeChange HandleEvent(const sf::Event& evt, sf::RenderWindow& win);

    void SetPosition(sf::Vector2f newPos);
private:
    sf::Sprite sprite;

    sf::Texture texMute;
    sf::Texture texLow;
    sf::Texture texMid;
    sf::Texture texHigh;

    sf::Text txtVolume;
    sf::RectangleShape volBackground;
    sf::RectangleShape volBar;
    sf::RectangleShape volIndicator;

    bool showSlider;
    bool isDragging;

    sf::Font font;
    bool fontLoaded;
};
