#include "SongRow.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SelectSongCommand.hpp"

void SongView::Init(AudioManager* audioRef) {
    this->audio = audioRef;
}

void SongView::SetSongs(const std::vector<SongRow>& songs) {
    items = songs;

    float y = 100.0f; 
    float x = 50.0f; 

    for (auto& item : items) {
        item.background.setSize(sf::Vector2f(800.0f, 30.0f));
        item.background.setPosition(0.0f, y);
        item.background.setFillColor(sf::Color(40, 40, 40, 0)); // Transparente o color suave

        item.title.setPosition(x, y + 5);

        item.artist.setPosition(x + 300, y + 5);

        item.duration.setPosition(x + 700, y + 5);

        y += 35.0f;
    }
}

void SongView::Draw(sf::RenderTarget& target) {
    for (const auto& item : items) {
        target.draw(item.background);
        target.draw(item.title);
        target.draw(item.artist); 
        target.draw(item.duration);
    }
}

void SongView::HandleClick(const sf::Vector2f& mousePos) {
    if (!isActive || !audio) return; // Seguridad

    for (const auto& item : items) {
        // Asumiendo que item.background es el área clickeable
        if (item.background.getGlobalBounds().contains(mousePos)) {
            if (item.song) {
                // Ejecutamos el comando directamente
                SelectSongCommand cmd(*item.song, *audio);
                cmd.Execute();
                return; // Ya encontramos la canción, salimos
            }
        }
    }
}