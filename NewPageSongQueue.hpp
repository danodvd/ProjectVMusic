#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Page.hpp"
#include "Command.hpp"
#include "NewAudioManager.hpp"
#include "NewID3Reader.hpp"
#include "SongData.hpp"
#include "PlayBackInfo.hpp"

/**
* Represents the visual representation of a song in the queue.
*
* This structure contains all rendering-related data needed to draw
* a song entry and references the underlying SongData model.
*/
struct QueueItemView {
    const SongData* song;
    sf::RectangleShape box;
    sf::Text title;
    sf::Text artist;
    sf::Text album;
    sf::Text year;
    sf::Text genre;
    sf::Text duration;
    UUID indexInPlaylist;
};

struct SongData; // Forward declaration.
/**
* UI page that displays and manages the current song queue.
*
* PageSongQueue is responsible for creating and updating the visual
* representation of the queue based on the data provided by AudioManager.
* User input is handled by the base Page class and dispatched via Commands.
*/
class PageSongQueue : public Page {
public:
    PageSongQueue(AudioManager& audioRef);

    /**
    * Rebuilds the visual queue items from the current playlist.
    *
    * This method recreates all QueueItemView instances to reflect the
    * current state of the queue stored in AudioManager.
    *
    * @param audio AudioManager used as the source of song data.
    */
    void RefreshQueue();

    void HandleClick(const sf::Vector2f& mousePos);

    void Update() override;

    void Draw(sf::RenderTarget& target);

private:
    /// Visual representations of the songs currently in the queue.
    /// This container is rebuilt whenever the queue changes.
    std::vector<QueueItemView> items;

    sf::Text pageTitle;

    sf::RectangleShape addSongsBox;
    sf::Text addSongsText;

    sf::RectangleShape deleteQueueBox;
    sf::Text deleteQueueText;

    const sf::Color Q_BG_NORMAL = sf::Color(45, 45, 50);
    const sf::Color Q_BG_HOVER = sf::Color(60, 60, 65);
    const sf::Color Q_TXT_NORMAL = sf::Color(230, 230, 230);
    const sf::Color Q_TXT_ACTIVE = sf::Color(255, 140, 0);
    const sf::Color Q_TXT_DIM = sf::Color(160, 160, 160);

    // --- Configuración Visual (Dark Mode) ---
    const float ROW_HEIGHT = 50.f;
    const float ROW_SPACING = 5.f;
    const float START_Y = 100.f;

    // Columnas
    const float COL_TITLE = 50.f;
    const float COL_ALBUM = 400.f;
    const float COL_ARTIST = 650.f;
    const float COL_YEAR = 850.f;
    const float COL_GENRE = 950.f;
    const float COL_DURATION = 1100.f;

    // Helpers internos
    std::string FormatTime(double seconds);
};