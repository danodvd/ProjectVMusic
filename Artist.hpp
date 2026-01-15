#pragma once
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SubView.hpp"

struct SongData; // Forward declaration
class PageMyMusic;

struct ArtistData {
	std::string name;
	std::vector<const SongData*> songs;
};

struct ArtistViewItem {
	 ArtistData artist;

	sf::Text title;
	sf::RectangleShape box;
    sf::Texture coverTexture;
    bool hasCover = false;
};

class PageMyMusic; // Forward declaration.

class ArtistView : public SubView { // Heredar de SubView
public:
    void Init(PageMyMusic* parentPage, AudioManager* audioRef);
    void SetArtists(const std::vector<ArtistData>& artists);
    void Draw(sf::RenderTarget& target) override;
    void HandleClick(const sf::Vector2f& mousePos) override;

    void SetActive() override { isActive = true; }
    void SetInactive() { isActive = false; }

private:
    bool isActive = false;
    std::vector<ArtistViewItem> items;
    PageMyMusic* parent = nullptr;
    AudioManager* audio = nullptr;
};