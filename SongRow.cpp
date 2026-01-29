#include "SongRow.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SelectSongCommand.hpp"
#include "UIUtils.hpp"

void SongView::Init(AudioManager* audioRef) {
    this->audio = audioRef;
}

void SongView::SetSongs(const std::vector<SongRow>& songs) {
    items = songs;

    for (size_t i = 0; i < items.size(); ++i) {
        auto& item = items[i];

        sf::Color bgColor = (i % 2 == 0) ? BACKGROUND_COLOR1 : BACKGROUND_COLOR2;

        float rowY = START_Y + i * ITEM_SPACING;

        item.title.setPosition(START_X, rowY);
        item.artist.setPosition(START_X + 300.0f, rowY);
        item.duration.setPosition(START_X + 700.0f, rowY);

        float bgHeight = ITEM_SPACING;
        float bgY = rowY - bgHeight / 2.0f;

        item.background.setSize(sf::Vector2f(800.0f, bgHeight));
        item.background.setPosition(0.0f, bgY);
        item.background.setFillColor(bgColor);
    }
}


void SongView::UpdateLayout(float width) {

    float colTitleX = width * 0.05f;   
    float colTitleW = width * 0.45f;     

    float colArtistX = width * 0.55f;    
    float colArtistW = width * 0.30f;    

    float colDurX = width * 0.90f;   

    int i = 0;
    for (auto& item : items) {
        sf::FloatRect textBounds = item.title.getLocalBounds();
        float textHeight = textBounds.height;

        float posY = item.title.getPosition().y;

        float bgHeight = ITEM_SPACING;

        float bgY = posY + textHeight / 2 - bgHeight / 2;

        float rowY = START_Y + (i * ITEM_SPACING);

        item.background.setSize(sf::Vector2f(width * 0.95f, bgHeight));
        item.background.setPosition(width * 0.025f, bgY);

        item.title.setString(item.song->title.empty() ? "Desconocido" : item.song->title);
        item.title.setPosition(colTitleX, rowY);
        UIUtils::ClipText(item.title, colTitleW);

        item.artist.setString(item.song->artist.empty() ? "Desconocido" : item.song->artist);
        item.artist.setPosition(colArtistX, rowY);
        UIUtils::ClipText(item.artist, colArtistW); 

        item.duration.setPosition(colDurX, rowY);

        i++;
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

void SongView::Update() {
    const SongData* currentSong = audio->GetCurrentSong();

    for (auto& item : items) {
        if (item.song == currentSong) {
            item.title.setFillColor(TXT_COLOR_ACTIVE);
            item.artist.setFillColor(TXT_COLOR_ACTIVE);
            item.duration.setFillColor(TXT_COLOR_ACTIVE);
        }
        else {
            item.title.setFillColor(TXT_COLOR_NORMAL);
            item.artist.setFillColor(TXT_COLOR_NORMAL);
            item.duration.setFillColor(TXT_COLOR_NORMAL);
        }
    }
}

void SongView::HandleClick(const sf::Vector2f& mousePos) {
    if (!isActive || !audio) return;

    for (auto& item : items) {
        if (item.background.getGlobalBounds().contains(mousePos)) {
            if (item.song) {
                SelectSongCommand cmd(*item.song, *audio);
                cmd.Execute();
            }
        }
    }
}