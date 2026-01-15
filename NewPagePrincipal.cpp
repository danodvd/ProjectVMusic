#define NOMINMAX

#include "NewPagePrincipal.hpp"
#include "UIUtils.hpp"
#include "SongImportCommand.hpp"
#include <windows.h>

PagePrincipal::PagePrincipal(AudioManager& audioRef) : Page(audioRef) {
	rPanelIsActive = false;
	hasCover = false;

	UIUtils::ConfigureLabel(title, 18, { 60, 20 }, "Página Principal");
	UIUtils::ConfigureLabel(txtMusic, 18, { 590, 20 }, "Seleccionar Canción");
	
	UIUtils::ConfigureBox(boxMusic, { 210, 40 }, { txtMusic.getPosition().x - 15, txtMusic.getPosition().y - 10 }, sf::Color(255, 255, 255, 30), sf::Color::White);
	UIUtils::ConfigureBox(rightPanel, { 250,630 }, { 1030, 0 }, sf::Color(40, 40, 40, 120), sf::Color::Transparent, 0);

	rPanelGradient = Page::SetBackgroundGradient(rightPanel);

	UIUtils::ConfigureBox(artPlaceHolder, { 200, 200 }, { 1055, 50 }, sf::Color::Transparent, sf::Color(100, 100, 100), 2);

	artSprite.setPosition(1055, 50);

	UIUtils::ConfigureLabel(lblSongTitle, 16, { 1055, 270 }, "Sin Canción");
	UIUtils::ConfigureLabel(lblArtist, 12, { 1055, 300 }, "Desconocido", sf::Color(180, 180, 180));

	Button musicButton;
	musicButton.bounds = boxMusic.getGlobalBounds();
	musicButton.command = std::make_unique<ImportSongCommand>(audioRef, GetActiveWindow());
	buttons.push_back(std::move(musicButton));
}

void PagePrincipal::UpdateInfo(const SongData& data) {
	std::string songName = data.title;
	std::string artistName = data.artist;
	std::vector<unsigned char> imgData = data.coverImage;
	if (songName.length() > 25) songName = songName.substr(0, 22) + "...";
	lblSongTitle.setString(songName);
	lblArtist.setString(artistName);

	hasCover = false;
	if (!imgData.empty()) {
		if (artTexture.loadFromMemory(imgData.data(), imgData.size())) {
			hasCover = true;
			artSprite.setTexture(artTexture);

			sf::Vector2u size = artTexture.getSize();
			float scaleX = 200.0f / size.x;
			float scaleY = 200.0f / size.y;
			artSprite.setScale(scaleX, scaleY);
		}
	}
}

void PagePrincipal::ToggleRPanel(const PlaybackInfo& info) {
	if (!info.hasStopped) rPanelIsActive = true;
	else rPanelIsActive = false;
}

void PagePrincipal::UpdateVisualizer(const float* fftData) {
	visualizer.UpdateVisualizer(fftData);
}

void PagePrincipal::Draw(sf::RenderTarget& target) {
	// 1. DIBUJAR FONDO UNIVERSAL PRIMERO
	if (hasCustomBg) {
		target.draw(bgSprite);
	}
	else {
		Page::DrawBackground(target); // <--- ESTO APLICA EL DEGRADADO
	}

	target.draw(title);


	float visLimit = rPanelIsActive ? rightPanel.getPosition().x : (float)target.getSize().x;
	visualizer.Draw(target, visLimit);

	if (rPanelIsActive) {
		sf::RenderStates states;
		states.transform.translate(rightPanel.getPosition());
		target.draw(rPanelGradient, states);
		target.draw(rPanelGradient, states);
		target.draw(rightPanel);

		if (hasCover) target.draw(artSprite);
		else target.draw(artPlaceHolder);

		target.draw(lblSongTitle);
		target.draw(lblArtist);

	}

	target.draw(boxMusic);
	target.draw(txtMusic);

}

void PagePrincipal::Update(const PlaybackInfo& info) {
	ToggleRPanel(info); // Lógica interna

	if (info.isPlaying) {
		// 1. LÓGICA DEL VISUALIZADOR (Lo que faltaba)
			float fftData[1024];
			// 'audio' es accesible porque heredas de Page
			audio.GetFFT(fftData);
			visualizer.UpdateVisualizer(fftData);

		// 2. LÓGICA DE ACTUALIZACIÓN DE DATOS (Título, Artista, Cover)
		// Verificamos si hay una canción sonando actualmente
		const SongData* currentSong = audio.GetCurrentSong();

		if (currentSong) {
			if (currentSong->title != lastSongTitle) {
				lastSongTitle = currentSong->title;
				UpdateInfo(*currentSong);          
			}
		}
	}
}

void PagePrincipal::UpdateLayout(sf::Vector2u newSize) {
	Page::UpdateLayout(newSize);

	float w = (float)newSize.x;
	float h = (float)newSize.y;

	// Ancho fijo del panel derecho
	float panelW = 250.f;
	float panelX = w - panelW; // Empieza donde termina la pantalla menos 250px

	rightPanel.setSize({ panelW, h - 90}); // Alto completo
	rightPanel.setPosition(panelX, 0);

	rPanelGradient = Page::SetBackgroundGradient(rightPanel);

	float btnW = 210.f;
	float btnX = panelX + (panelW - btnW) / 2.f;

	UIUtils::ConfigureLabel(txtMusic, 18, { btnX + 15, 30 }, "Seleccionar Canción");
	UIUtils::ConfigureBox(boxMusic, { btnW, 40 }, { btnX, 20 }, sf::Color(255, 255, 255, 30), sf::Color::White);

	if (!buttons.empty()) buttons[0].bounds = boxMusic.getGlobalBounds();

	float textMargin = 20.f;

	artSprite.setPosition(panelX + (panelW - 200) / 2, 80);
	artPlaceHolder.setPosition(panelX + (panelW - 200) / 2, 80);

	lblSongTitle.setPosition(panelX + textMargin, 300);
	lblArtist.setPosition(panelX + textMargin, 330); // "Release - Topic" ahora seguirá al panel

	float visualizerCenter = (w - panelW) / 2.f;
}