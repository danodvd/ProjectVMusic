#include "NewPageSongQueue.hpp"
#include "UIUtils.hpp"
#include "Utils.hpp"
#include "SelectSongCommand.hpp"
#include "SongImportCommand.hpp"
#include "DeleteQueueCommand.hpp"

PageSongQueue::PageSongQueue(AudioManager& audioRef) : Page(audioRef)
{
    UIUtils::ConfigureLabel(pageTitle, 36, { 40, 30 }, "Cola de Reproducción");
    pageTitle.setStyle(sf::Text::Bold);

    UIUtils::ConfigureLabel(addSongsText, 18, { 600, 50 }, "Añadir Canciones");
    addSongsText.setStyle(sf::Text::Bold);
    UIUtils::ConfigureBox(addSongsBox, { 200, 40 }, { addSongsText.getPosition().x - 20, addSongsText.getPosition().y - 10 }, sf::Color(255, 255, 255, 30), sf::Color::White);

    UIUtils::ConfigureLabel(deleteQueueText, 18, { 450, 50 }, "Borrar Cola");
    deleteQueueText.setStyle(sf::Text::Bold);
    UIUtils::ConfigureBox(deleteQueueBox, { 150, 40 }, { deleteQueueText.getPosition().x - 20, deleteQueueText.getPosition().y - 10 }, sf::Color(255, 255, 255, 30), sf::Color::White);

    Button btnAdd;
    btnAdd.bounds = addSongsBox.getGlobalBounds();
    // Reutilizamos el comando de importar
    btnAdd.command = std::make_unique<ImportSongCommand>(audioRef, GetActiveWindow());
    buttons.push_back(std::move(btnAdd));

    Button btnDeleteQueue;
    btnDeleteQueue.bounds = deleteQueueBox.getGlobalBounds();
    btnDeleteQueue.command = std::make_unique<DeleteQueueCommand>(audioRef, items);
    buttons.push_back(std::move(btnDeleteQueue));
    RefreshQueue();
}

void PageSongQueue::Update() {
    RefreshQueue();
}

void PageSongQueue::RefreshQueue() {
    items.clear();

    const std::vector<SongData>& playlist = Page::audio.GetQueue();

    const SongData* currentSong = Page::audio.GetCurrentSong();

    float currentY = START_Y;

    for (size_t i = 0; i < playlist.size(); ++i) {
        const SongData& data = playlist[i];

        bool isActive = (currentSong == &data);

        QueueItemView item;

        item.song = &data;

        item.box.setSize(sf::Vector2f(1200.f, ROW_HEIGHT));
        item.box.setPosition(20.f, currentY);
        item.box.setFillColor(isActive ? Q_BG_HOVER : Q_BG_NORMAL);

        sf::Color txtColor = isActive ? Q_TXT_ACTIVE : Q_TXT_NORMAL;
        sf::Color dimColor = isActive ? Q_TXT_ACTIVE : Q_TXT_DIM;

        UIUtils::ConfigureLabel(item.title, 14, { COL_TITLE, currentY + 10 }, data.title, txtColor);
        UIUtils::ConfigureLabel(item.album, 14, { COL_ALBUM, currentY + 10 }, data.album, dimColor);
        UIUtils::ConfigureLabel(item.artist, 14, { COL_ARTIST, currentY + 10 }, data.artist, dimColor);
        UIUtils::ConfigureLabel(item.year, 14, { 850.f, currentY + 10 }, std::to_string(data.year), dimColor);
        UIUtils::ConfigureLabel(item.genre, 14, { 950.f, currentY + 10 }, data.genre, dimColor);
        UIUtils::ConfigureLabel(item.duration, 14, { 1100.f, currentY + 10 }, Utils::FormatDuration(data.duration), txtColor);

        items.push_back(item);

        currentY += ROW_HEIGHT + ROW_SPACING;
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