#include "Album.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "NewPageMyMusic.hpp"
#include "UIUtils.hpp"

void AlbumView::SetAlbums(const std::vector<AlbumData>& albums) {
    items.clear();
    items.reserve(albums.size());

    float x = 50.0f;
    float y = 100.0f;
    float boxSize = 140.f;

    for (const auto& alb : albums) {
        items.emplace_back();
        AlbumViewItem& item = items.back(); 

        item.album = alb;

        UIUtils::ConfigureBox(item.box, { boxSize, boxSize }, { x, y }, sf::Color(50, 50, 50));
        item.box.setOutlineThickness(2.f);
        item.box.setOutlineColor(sf::Color(100, 100, 100));

        if (!item.album.songs.empty()) {
            const std::vector<unsigned char>& imgData = item.album.songs[0]->coverImage;

            if (!imgData.empty()) {
                if (item.coverTexture.loadFromMemory(imgData.data(), imgData.size())) {
                    item.hasCover = true;
                    item.coverTexture.setSmooth(true);

                    item.box.setTexture(&item.coverTexture, true);
                    item.box.setFillColor(sf::Color::White);
                }
            }
        }

        std::string titleStr(item.album.name.begin(), item.album.name.end());
        if (titleStr.empty()) titleStr = "Desconocido";
        if (titleStr.length() > 18) titleStr = titleStr.substr(0, 15) + "...";

        UIUtils::ConfigureLabel(item.title, 14, { x, y + boxSize + 10 }, titleStr);

        sf::FloatRect textBounds = item.title.getLocalBounds();
        float textX = x + (boxSize - textBounds.width) / 2.f;
        item.title.setPosition(textX, y + boxSize + 10);

        x += 170.0f;
        if (x > 750.0f) {
            x = 50.0f;
            y += 200.0f;
        }
    }
}

void AlbumView::Draw(sf::RenderTarget& target) {
    for (const auto& item : items) {
        target.draw(item.box);
        target.draw(item.title);
    }
}

void AlbumView::Init(PageMyMusic* parentPage, AudioManager* audioRef) {
    parent = parentPage;
    audio = audioRef;
}

void AlbumView::HandleClick(const sf::Vector2f& mousePos) {
    if (!isActive) return;

    for (AlbumViewItem& item : items) { 
        if (item.box.getGlobalBounds().contains(mousePos)) {
            if (parent) {
                parent->OpenAlbum(&item.album);
                return;
            }
        }
    }
}

void AlbumView::SetActive() {
    this->isActive = true;
}