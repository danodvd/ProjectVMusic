#pragma once
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SubView.hpp"

struct SongData; // Forward declaration
class PageMyMusic;
/**
* Represents the album data containing the songs that share the same album data.
*/
struct AlbumData {
	std::string name;
	std::vector<const SongData*> songs;
};

/**
* Represents a visual representation of the album.
*/
struct AlbumViewItem {
	AlbumData album;

	sf::Text title;
	sf::RectangleShape box;
	sf::Texture coverTexture;
	bool hasCover = false;
};

class PageMyMusic; // Forward declaration.

/**
* Represents the internal page of PageMyMusic that displays all the available albums.
* 
* Contains a collection of clickable Album elements in display, each of which has its
* own page.
*/
class AlbumView : public SubView {
public:
	void Init(PageMyMusic* parentPage, AudioManager* audioRef);

    void SetAlbums(const std::vector<AlbumData>& albums);

    void Draw(sf::RenderTarget& target) override;

    void HandleClick(const sf::Vector2f& mousePos) override;

	void SetActive() override;
	void SetInactive() { isActive = false; }

private:
    bool isActive = false;
    std::vector<AlbumViewItem> items;
	PageMyMusic* parent = nullptr;
	AudioManager* audio = nullptr;
};