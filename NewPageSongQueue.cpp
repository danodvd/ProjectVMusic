#include "NewPageSongQueue.hpp"
#include "UIUtils.hpp"
#include "Utils.hpp"
#include "SelectSongCommand.hpp"
#include "SongImportCommand.hpp"
#include "DeleteQueueCommand.hpp"

PageSongQueue::PageSongQueue(AudioManager& audioRef) : Page(audioRef)
{
    UIUtils::ConfigureLabel(pageTitle, 36, { 80, 7 }, "Cola de Reproducción");
    pageTitle.setStyle(sf::Text::Bold);

    UIUtils::ConfigureLabel(addSongsText, 18, { 600, 50 }, "Añadir Canciones");
    addSongsText.setStyle(sf::Text::Bold);
    UIUtils::ConfigureBox(addSongsBox, { 200, 40 }, { addSongsText.getPosition().x - 20, addSongsText.getPosition().y - 10 }, sf::Color(255, 255, 255, 30), sf::Color::White);

    UIUtils::ConfigureLabel(deleteQueueText, 18, { 450, 50 }, "Borrar Cola");
    deleteQueueText.setStyle(sf::Text::Bold);
    UIUtils::ConfigureBox(deleteQueueBox, { 150, 40 }, { deleteQueueText.getPosition().x - 20, deleteQueueText.getPosition().y - 10 }, sf::Color(255, 255, 255, 30), sf::Color::White);

    Button btnAdd;
    btnAdd.bounds = addSongsBox.getGlobalBounds();
    btnAdd.command = std::make_unique<ImportSongCommand>(audioRef, GetActiveWindow());
    buttons.push_back(std::move(btnAdd));

    Button btnDeleteQueue;
    btnDeleteQueue.bounds = deleteQueueBox.getGlobalBounds();
    btnDeleteQueue.command = std::make_unique<DeleteQueueCommand>(audioRef, items);
    buttons.push_back(std::move(btnDeleteQueue));

    RefreshQueue();
}

void PageSongQueue::Update() {
    if (lastQueueSize != audio.GetQueue().size()) {
        RefreshQueue();
    }
    else {
        UpdateStyles();
    }

}

void PageSongQueue::RefreshQueue() {
    items.clear();

    const std::vector<SongData>& playlist = Page::audio.GetQueue();
    lastQueueSize = playlist.size();

    for (size_t i = 0; i < playlist.size(); ++i) {
        const SongData& data = playlist[i];
        QueueItemView item;
        item.song = &data;

        UIUtils::ConfigureLabel(item.title, 14, { 0, 0 }, data.title);
        UIUtils::ConfigureLabel(item.album, 14, { 0, 0 }, data.album);
        UIUtils::ConfigureLabel(item.artist, 14, { 0, 0 }, data.artist);
        UIUtils::ConfigureLabel(item.year, 14, { 0, 0 }, std::to_string(data.year));
        UIUtils::ConfigureLabel(item.genre, 14, { 0, 0 }, data.genre);
        UIUtils::ConfigureLabel(item.duration, 14, { 0, 0 }, Utils::FormatDuration(data.duration));

        item.box.setFillColor(sf::Color::Transparent);

        items.push_back(item);
    }

    if (commonBackground.getVertexCount() > 2) {
        sf::Vector2u size((unsigned int)commonBackground[2].position.x, (unsigned int)commonBackground[2].position.y);
        UpdateLayout(size);
    }
}


void PageSongQueue::UpdateStyles() {
    const SongData* currentSong = Page::audio.GetCurrentSong();

    int i = 0;
    for (auto& item : items) {
        bool isActive = (item.song == currentSong);

        sf::Color bgColor = (i % 2 == 0) ? BACKGROUND_COLOR1 : BACKGROUND_COLOR2;
        if (isActive) bgColor = Q_BG_HOVER;

        item.box.setFillColor(bgColor);

        sf::Color txtColor = isActive ? Q_TXT_ACTIVE : Q_TXT_NORMAL;
        sf::Color dimColor = isActive ? Q_TXT_ACTIVE : Q_TXT_DIM;

        item.title.setFillColor(txtColor);
        item.artist.setFillColor(dimColor);
        item.album.setFillColor(dimColor);
        item.duration.setFillColor(txtColor);
        item.genre.setFillColor(txtColor);
        item.year.setFillColor(dimColor);

        ++i;
    }
}

void PageSongQueue::UpdateLayout(sf::Vector2u newSize) {
    Page::UpdateLayout(newSize);

    float width = (float)newSize.x;
    float startY = 100.0f;

    float colTitleX = width * 0.05f;   float colTitleW = width * 0.30f;
    float colArtistX = width * 0.40f;  float colArtistW = width * 0.20f;
    float colAlbumX = width * 0.65f;   float colAlbumW = width * 0.20f;
    float colDurX = width * 0.90f;

    // Add year, genre

    float w = static_cast<float>(newSize.x);
    float centerX = w / 2.0f;
    float spacing = 250.f;
    float y = 25.f;

    float titleRightEdge = pageTitle.getPosition().x + pageTitle.getGlobalBounds().width + 40.f;

    float idealStartX = centerX - (spacing * 0.5f);

    float startX = std::max(idealStartX, titleRightEdge);

    std::vector<std::pair<sf::Text*, sf::RectangleShape*>> boxes = {
        { &addSongsText,    &addSongsBox },
        { &deleteQueueText,  &deleteQueueBox },
    };

    for (size_t i = 0; i < boxes.size(); ++i)
    {
        auto& [text, box] = boxes[i];

        text->setPosition(startX + spacing * i, y);

        box->setPosition(text->getPosition().x - 30, y - 10);

        if (i < buttons.size()) {
            buttons[i].bounds = box->getGlobalBounds();
        }
    }
    int i = 0;
    for (auto& item : items) {
        float rowY = startY + (i * ITEM_SPACING);

        item.box.setSize(sf::Vector2f(width * 0.95f, ITEM_SPACING - 5.f));
        item.box.setPosition(width * 0.025f, rowY - 5.f);

        item.title.setString(item.song->title.empty() ? "Desconocido" : item.song->title);
        item.title.setPosition(colTitleX, rowY);
        UIUtils::ClipText(item.title, colTitleW);

        item.artist.setString(item.song->artist.empty() ? "Desconocido" : item.song->artist);
        item.artist.setPosition(colArtistX, rowY);
        UIUtils::ClipText(item.artist, colArtistW);

        item.album.setString(item.song->album);
        item.album.setPosition(colAlbumX, rowY);
        UIUtils::ClipText(item.album, colAlbumW);

        item.duration.setPosition(colDurX, rowY);

        // Pending addition: Year, genre

        ++i;
    }
}

void PageSongQueue::Draw(sf::RenderTarget& target) {
    Page::DrawBackground(target);
    target.draw(pageTitle);
    target.draw(addSongsText);
    target.draw(deleteQueueText);
    target.draw(addSongsBox);
    target.draw(deleteQueueBox);

    for (const auto& item : items) {
        target.draw(item.box);
        target.draw(item.title);
        target.draw(item.album);
        target.draw(item.artist);
        target.draw(item.year);
        target.draw(item.genre);
        target.draw(item.duration);
    }
}

void PageSongQueue::HandleClick(const sf::Vector2f& mousePos) {
    Page::HandleClick(mousePos);

    for (const auto& item : items) {
        if (item.box.getGlobalBounds().contains(mousePos)) {
            SelectSongCommand tempCmd(*item.song, Page::audio);
            tempCmd.Execute();
            return; 
        }
    }
}