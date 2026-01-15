#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Page.hpp"
#include "SongData.hpp"
#include "PlayBackInfo.hpp"
#include "AudioVisualizerWidget.hpp"

/**
* Represents the visual representation of the main page of the application.
* 
* This structure contains all rendering-related data needed to draw
* the main page items.
*/
class PagePrincipal : public Page {
public:
    PagePrincipal(AudioManager& audioRef);

    /**
    * Updates the display information when a new song starts playing.
    * 
    * @param songName The name of the new song.
    * 
    * @param artistName The name of the new artist.
    * 
    * @param imgData The image data of the selected song.
    */
    void UpdateInfo(const SongData& data);

    void UpdateVisualizer(const float* fftData);

    void Draw(sf::RenderTarget& target) override;

    void Update(const PlaybackInfo& info) override;

    void UpdateLayout(sf::Vector2u newSize) override;

    /**
    * Toggles the right panel, which displays the current song data such as name, artist, duration and image cover.
    */
    void ToggleRPanel(const PlaybackInfo& info);

private:
    AudioVisualizerWidget visualizer;
    sf::RectangleShape bar;

    sf::Text title;
    sf::Text txtMusic;
    sf::RectangleShape boxMusic;
    sf::VertexArray rPanelGradient;

    sf::Texture bgTexture;
    sf::Sprite bgSprite;
    bool hasCustomBg = false;

    sf::RectangleShape rightPanel;
    bool rPanelIsActive = false;

    sf::RectangleShape artPlaceHolder;
    sf::Sprite artSprite;
    sf::Texture artTexture;
    bool hasCover = false;

    sf::Text lblSongTitle;
    sf::Text lblArtist;

    std::string lastSongTitle = "";
};