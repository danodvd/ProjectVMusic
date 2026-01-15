#include "Artist.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "NewPageMyMusic.hpp"
#include "UIUtils.hpp"

void ArtistView::Init(PageMyMusic* parentPage, AudioManager* audioRef) {
    parent = parentPage;
    audio = audioRef;
}

void ArtistView::SetArtists(const std::vector<ArtistData>& artists) {
    items.clear();
    items.reserve(artists.size());

    float x = 50.0f;
    float y = 100.0f;
    float boxSize = 140.f;

    for (const auto& art : artists) {
        items.emplace_back();
        ArtistViewItem& item = items.back();

        item.artist = art;

        UIUtils::ConfigureBox(item.box, { boxSize, boxSize }, { x, y }, sf::Color(50, 50, 50));
        item.box.setOutlineThickness(2.f);
        item.box.setOutlineColor(sf::Color(100, 100, 100));

        if (!item.artist.songs.empty()) {
            const std::vector<unsigned char>& imgData = item.artist.songs[0]->coverImage;

            if (!imgData.empty()) {
                if (item.coverTexture.loadFromMemory(imgData.data(), imgData.size())) {
                    item.hasCover = true;
                    item.coverTexture.setSmooth(true);

                    item.box.setTexture(&item.coverTexture, true);
                    item.box.setFillColor(sf::Color::White);
                }
            }
        }

        std::string titleStr(item.artist.name.begin(), item.artist.name.end());
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

void ArtistView::Draw(sf::RenderTarget& target) {
    for (const auto& item : items) {
        target.draw(item.box);
        target.draw(item.title);
    }
}

void ArtistView::HandleClick(const sf::Vector2f& mousePos) {
    if (!isActive) return;

    for (ArtistViewItem& item : items) {
        if (item.box.getGlobalBounds().contains(mousePos)) {
            if (parent) {
                parent->OpenArtist(&item.artist);
                return;
            }
        }
    }
}