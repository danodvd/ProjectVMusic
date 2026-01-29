#include "NewPageMyMusic.hpp"
#include "UIUtils.hpp"
#include "FileUtils.hpp"
#include "PlatformUtils.hpp"
#include "NewID3Reader.hpp"
#include "ChangeSubViewCommand.hpp"
#include "SongImportCommand.hpp"
#include "Utils.hpp"
#include <map>

PageMyMusic::PageMyMusic(AudioManager& audioRef) : Page(audioRef) {
	lastSongCount = 0;

	UIUtils::ConfigureLabel(title, 36, { 80, 7 }, "Mi Música");

	UIUtils::ConfigureLabel(txtSongs, 18, { 200, 25 }, "Canciones");
	UIUtils::ConfigureLabel(txtArtists, 18, { 350, 25 }, "Artistas");
	UIUtils::ConfigureLabel(txtAlbums, 18, { 500, 25 }, "Álbums");
	UIUtils::ConfigureLabel(txtAddSongs, 18, { 650, 25 }, "Agregar Música");

	UIUtils::ConfigureBox(boxAddSongs, { 180, 40 }, { txtAddSongs.getPosition().x - 30, txtAddSongs.getPosition().y - 10 }, sf::Color(255, 255, 255, 40), sf::Color::White);
	UIUtils::ConfigureBox(boxArtists, { 150, 40 }, { txtArtists.getPosition().x - 30, txtArtists.getPosition().y - 10 }, sf::Color(255, 255, 255, 40));
	UIUtils::ConfigureBox(boxAlbums, { 150, 40 }, { txtAlbums.getPosition().x - 30, txtAlbums.getPosition().y - 10 }, sf::Color(255, 255, 255, 40));
	UIUtils::ConfigureBox(boxSongs, { 150, 40 }, { txtSongs.getPosition().x - 30, txtSongs.getPosition().y - 10 }, sf::Color(255, 255, 255, 40));
	
	albumView.Init(this, &audio);
	artistView.Init(this, &audio);
	songView.Init(&audio);

	Button btnSongs;
	btnSongs.bounds = boxSongs.getGlobalBounds();
	btnSongs.command = std::make_unique<ChangeSubViewCommand>(*this, SubView::Songs);
	buttons.push_back(std::move(btnSongs));

	Button btnAlbums;
	btnAlbums.bounds = boxAlbums.getGlobalBounds();
	btnAlbums.command = std::make_unique<ChangeSubViewCommand>(*this, SubView::Albums);
	buttons.push_back(std::move(btnAlbums));

	Button btnArtists;
	btnArtists.bounds = boxArtists.getGlobalBounds();
	btnArtists.command = std::make_unique<ChangeSubViewCommand>(*this, SubView::Artists);
	buttons.push_back(std::move(btnArtists));

	Button btnAdd;
	btnAdd.bounds = boxAddSongs.getGlobalBounds();
	btnAdd.command = std::make_unique<ImportSongCommand>(audioRef, GetActiveWindow());
	buttons.push_back(std::move(btnAdd));

	UpdateCurrentView();
	Update();
}

std::vector<SongRow> PageMyMusic::CreateSongRows(const std::vector<const SongData*>& sourceData) {
	std::vector<SongRow> rows;
	rows.reserve(sourceData.size());

	for (const auto* song : sourceData) {
		SongRow row;
		UIUtils::ConfigureLabel(row.title, 14, { 0,0 }, song->title.empty() ? "Desconocido" : song->title);
		UIUtils::ConfigureLabel(row.artist, 14, { 0,0 }, song->artist.empty() ? "Desconocido" : song->artist, sf::Color(200, 200, 200));
		UIUtils::ConfigureLabel(row.duration, 14, { 0,0 }, Utils::FormatDuration(song->duration));

		row.background.setFillColor(sf::Color(255, 255, 255, 5));
		row.song = song;
		rows.push_back(row);
	}
	return rows;
}

void PageMyMusic::Refresh() {
	library.SetLibraryData(audio.GetQueue());

	UpdateCurrentView();
}

void PageMyMusic::Update() {
	size_t currentCount = audio.GetQueue().size();

	if (currentCount != lastSongCount) {
		lastSongCount = currentCount; 
		Refresh();                   
	}

	songView.Update();
}

void PageMyMusic::UpdateLayout(sf::Vector2u newSize)
{
	Page::UpdateLayout(newSize);

	float w = static_cast<float>(newSize.x);
	float centerX = w / 2.0f;

	float spacing = 150.f;
	float y = 25.f;
	float startX = centerX - (spacing * 1.5f);

	std::vector<std::pair<sf::Text*, sf::RectangleShape*>> items = {
		{ &txtSongs,    &boxSongs },
		{ &txtArtists,  &boxArtists },
		{ &txtAlbums,   &boxAlbums },
		{ &txtAddSongs, &boxAddSongs }
	};

	for (size_t i = 0; i < items.size(); ++i)
	{
		auto& [text, box] = items[i];

		text->setPosition(startX + spacing * i, y);
		box->setPosition(text->getPosition().x - 30, y - 10);

		buttons[i].bounds = box->getGlobalBounds();
	}

	songView.UpdateLayout(w);
}

void PageMyMusic::SetCurrentView(SubView newView) {
	currentView = newView;
	
	UpdateCurrentView();
}

void PageMyMusic::UpdateCurrentView() {
	songView.SetInactive();
	albumView.SetInactive();
	artistView.SetInactive();

	switch (currentView) {
		case SubView::Songs:
			songView.SetSongs(CreateSongRows(library.GetSongsSortedByTitle()));
			if (commonBackground.getVertexCount() > 2) {
				float currentWidth = commonBackground[2].position.x;
				songView.UpdateLayout(currentWidth);
			}
			songView.SetActive();
			break;

		case SubView::Albums:
			albumView.SetAlbums(library.GetAlbumsSorted());
			albumView.SetActive();
			break;

		case SubView::Artists:
			artistView.SetArtists(library.GetArtistsSorted());
			artistView.SetActive();
			break;
	}
}

void PageMyMusic::OpenAlbum(const AlbumData* album) {
	songView.SetSongs(CreateSongRows(library.GetSongsFromAlbum(album)));
	currentView = SubView::Songs;

	songView.SetActive();
	albumView.SetInactive();
	artistView.SetInactive();
}

void PageMyMusic::OpenArtist(const ArtistData* artist) {
	songView.SetSongs(CreateSongRows(library.GetSongsFromArtist(artist)));
	currentView = SubView::Songs;

	songView.SetActive();
	albumView.SetInactive();
	artistView.SetInactive();
}

void PageMyMusic::Draw(sf::RenderTarget& target) {
	Page::DrawBackground(target);
	target.draw(title);
	target.draw(txtAddSongs);
	target.draw(txtSongs);
	target.draw(txtAlbums);
	target.draw(txtArtists);
	target.draw(boxAddSongs);
	switch (currentView)
	{
	case PageMyMusic::SubView::Songs:
		songView.Draw(target);
		break;
	case PageMyMusic::SubView::Albums:
		albumView.Draw(target);
		break;
	case PageMyMusic::SubView::Artists:
		artistView.Draw(target);
		break;
	}
}

void PageMyMusic::HandleClick(const sf::Vector2f& mousePos) {	
	Page::HandleClick(mousePos);

	switch (currentView) {
	case SubView::Songs:
		songView.HandleClick(mousePos);
		break;

	case SubView::Albums:
		albumView.HandleClick(mousePos);
		break;

	case SubView::Artists:
		artistView.HandleClick(mousePos);
		break;
	}
}