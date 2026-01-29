#define NOMINMAX

#include "NewPagePrincipal.hpp"
#include "UIUtils.hpp"
#include "SongImportCommand.hpp"
#include <windows.h>

PagePrincipal::PagePrincipal(AudioManager& audioRef) : Page(audioRef) {
	rPanelIsActive = false;
	hasCover = false;

	UIUtils::ConfigureLabel(title, 36, { 80, 7 }, "Página Principal");
	UIUtils::ConfigureLabel(txtMusic, 18, { 590, 20 }, "Seleccionar Canción");
	
	UIUtils::ConfigureBox(boxMusic, { 210, 40 }, { txtMusic.getPosition().x - 15, txtMusic.getPosition().y - 10 }, sf::Color(255, 255, 255, 30), sf::Color::White);
	UIUtils::ConfigureBox(rightPanel, { 250,630 }, { 1030, 0 }, sf::Color(40, 40, 40, 120), sf::Color::Transparent, 0);

	rPanelGradient = Page::SetBackgroundGradient(rightPanel, *currentTheme);

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
	fullTitleStr = data.title;
	fullArtistStr = data.artist;

	hasCover = false;
	std::vector<unsigned char> imgData = data.coverImage;
	if (!imgData.empty()) {
		if (artTexture.loadFromMemory(imgData.data(), imgData.size())) {
			hasCover = true;
			artSprite.setTexture(artTexture);

			float contentW = currentPanelW * 0.8f;

			sf::Vector2u size = artTexture.getSize();
			if (size.x > 0) {
				float scale = contentW / (float)size.x;
				artSprite.setScale(scale, scale);
			}
		}
	}

	RecalculateTextTruncation();
}

void PagePrincipal::ToggleRPanel(const PlaybackInfo& info) {
	if (!info.hasStopped) rPanelIsActive = true;
	else rPanelIsActive = false;
}

void PagePrincipal::UpdateVisualizer(const float* fftData) {
	visualizer.UpdateVisualizer(fftData);
}

void PagePrincipal::Draw(sf::RenderTarget& target) {
	if (hasCustomBg) {
		target.draw(bgSprite);
	}
	else {
		Page::DrawBackground(target); 
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
	ToggleRPanel(info);

	if (info.isPlaying) {
		float fftData[1024];
		audio.GetFFT(fftData);
		visualizer.UpdateVisualizer(fftData);
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

	float panelW = w / 3.0f;
	float panelX = w - panelW;

	currentPanelW = panelW;
	currentPanelX = panelX;

	rightPanel.setSize({ panelW, h });
	rightPanel.setPosition(panelX, 0);
	rPanelGradient = Page::SetBackgroundGradient(rightPanel, *currentTheme);

	float contentW = panelW * 0.8f;
	float contentX = panelX + (panelW * 0.1f);

	UIUtils::ConfigureBox(boxMusic, { contentW, 40 }, { contentX, 20 }, sf::Color(255, 255, 255, 30), sf::Color::White);

	sf::FloatRect txtBounds = txtMusic.getLocalBounds();
	float txtX = contentX + (contentW - txtBounds.width) / 2.0f;
	float txtY = 20.f + (40.f - txtBounds.height) / 2.f - 5.f; 
	txtMusic.setPosition(txtX, txtY);

	if (!buttons.empty()) buttons[0].bounds = boxMusic.getGlobalBounds();

	if (!buttons.empty()) buttons[0].bounds = boxMusic.getGlobalBounds();

	float artY = 80.f;

	artPlaceHolder.setSize({ contentW, contentW });
	artPlaceHolder.setPosition(contentX, artY);

	artSprite.setPosition(contentX, artY);

	if (artTexture.getSize().x > 0) {
		float scale = contentW / (float)artTexture.getSize().x;
		artSprite.setScale(scale, scale);
	}
	float textY = artY + contentW + 20.f;

	lblSongTitle.setPosition(contentX, textY);
	lblArtist.setPosition(contentX, textY + 30.f);

	RecalculateTextTruncation();
}

void PagePrincipal::RecalculateTextTruncation() {
	lblSongTitle.setString(fullTitleStr.empty() ? "Sin Canción" : fullTitleStr);
	lblArtist.setString(fullArtistStr.empty() ? "Desconocido" : fullArtistStr);

	float contentX = lblSongTitle.getPosition().x; 
	float panelRightEdge = currentPanelX + currentPanelW;
	float hardLimitX = panelRightEdge - (currentPanelW * 0.20f); 

	float maxTextWidth = hardLimitX - contentX;

	if (maxTextWidth < 50.f) maxTextWidth = 50.f;

	UIUtils::ClipText(lblSongTitle, maxTextWidth);
	UIUtils::ClipText(lblArtist, maxTextWidth);
}