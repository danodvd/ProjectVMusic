#pragma once
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SubView.hpp"


struct SongData;

struct SongRow {
    const SongData* song;

    sf::RectangleShape background;
    sf::Text title;
    sf::Text artist;
    sf::Text duration;
};

class SongView : public SubView { 
public:
    void Init(AudioManager* audioRef);

    void SetSongs(const std::vector<SongRow>& songs);

    void Draw(sf::RenderTarget& target) override;

    void HandleClick(const sf::Vector2f& mousePos) override;

    void Update();
    void UpdateLayout(float width);

    void SetActive() override { isActive = true; }
    void SetInactive() { isActive = false; }

private:
    bool isActive = false;
    std::vector<SongRow> items;
    AudioManager* audio = nullptr;

    static constexpr float START_Y = 100.f;
    static constexpr float START_X = 50.f;
    static constexpr float ITEM_SPACING = 50.f;
    static constexpr float BACKGROUND_POS_Y = 25.f;

    const sf::Color TXT_COLOR_ACTIVE = sf::Color(255, 127, 0);
    const sf::Color TXT_COLOR_NORMAL = sf::Color::White;


    const sf::Color BACKGROUND_COLOR1 = sf::Color(20, 20, 20, 80);
    const sf::Color BACKGROUND_COLOR2 = (sf::Color(40, 40, 40, 80));
    sf::Color CURRENT_COLOR;
};