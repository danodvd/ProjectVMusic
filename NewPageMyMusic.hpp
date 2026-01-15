#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "NewAudioManager.hpp"
#include "LibraryModel.hpp"
#include <vector>
#include "Page.hpp"
#include "Artist.hpp"
#include "Album.hpp"
#include "SongRow.hpp"

/**
* Represents a UI page that displays a list of songs, artists or albums.
* 
* Contains a collection of clickable buttons that provide sub-view management.
*/
class PageMyMusic : public Page {
public:
	enum class SubView {
		Songs,
		Albums,
		Artists
	};

	PageMyMusic(AudioManager& audioRef);

	void OpenAlbum(const AlbumData* album);
	void OpenArtist(const ArtistData* artist);

	void Refresh();

	void Update() override;

	void UpdateCurrentView();

	void SetCurrentView(SubView newView);

	void HandleClick(const sf::Vector2f& mousePos) override;

	void Draw(sf::RenderTarget& target) override;


	const SongData* selectedSong = nullptr;

private:
	std::vector<SongRow> CreateSongRows(const std::vector<const SongData*>& sourceData);
private:
	LibraryModel library;

	SongView songView;
	AlbumView albumView;
	ArtistView artistView;

	SubView currentView = SubView::Songs;

	sf::Text title;
	sf::Text txtSongs;
	sf::Text txtArtists;
	sf::Text txtAlbums;

	sf::RectangleShape boxSongs;
	sf::RectangleShape boxArtists;
	sf::RectangleShape boxAlbums; 


	sf::Text txtAddSongs;
	sf::RectangleShape boxAddSongs;

	sf::Text txtTitleHeader;  
	sf::Text txtArtistHeader;
	sf::Text txtAlbumHeader; 
	sf::Text txtYearHeader;
	sf::Text txtGenreHeader;
	sf::Text txtDurationHeader;

	sf::Text lblSongTitle, lblArtist, lblAlbum, lblYear, lblGenre, lblDuration;
	size_t lastSongCount = 0;
};