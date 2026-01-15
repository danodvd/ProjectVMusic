#pragma once
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "PlayBackInfo.hpp"
#include "UIUtils.hpp"

class InfoPanel {
public:
    InfoPanel();

    void LoadFont(std::string fontPath);
    void Update(const PlaybackInfo& info);
    void Draw(sf::RenderTarget& target);
private:
    sf::Text txtTitle;
    sf::Text txtDuration;
    sf::Font font;
    bool fontLoaded;
};