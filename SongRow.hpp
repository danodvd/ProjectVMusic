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

class SongView : public SubView { // Heredar de SubView
public:
    void Init(AudioManager* audioRef);
    void SetSongs(const std::vector<SongRow>& songs);
    void Draw(sf::RenderTarget& target) override;
    void HandleClick(const sf::Vector2f& mousePos) override;

    // Implementación necesaria para que no sea abstracta
    void SetActive() override { isActive = true; }
    void SetInactive() { isActive = false; }

private:
    bool isActive = false;
    std::vector<SongRow> items;
    AudioManager* audio = nullptr;
};