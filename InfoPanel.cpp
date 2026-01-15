#include "InfoPanel.hpp"
#include <sstream> // Añadir esta línea al inicio del archivo

InfoPanel::InfoPanel() : fontLoaded(false) {
}

void InfoPanel::LoadFont(std::string fontPath) {
    fontLoaded = font.loadFromFile(fontPath);
    if (!fontLoaded) return;

    // Cambiar de vector de punteros a vector de objetos
    std::vector<sf::Text*> texts{ &txtTitle, &txtDuration };
    UIUtils::SetFont(font, texts);


    txtTitle.setCharacterSize(20);
    txtTitle.setFillColor(sf::Color::White);
    txtTitle.setPosition(20, 450);

    txtDuration.setCharacterSize(14);
    txtDuration.setFillColor(sf::Color(200, 200, 200));
    txtDuration.setPosition(700, 470);
}

void InfoPanel::Update(const PlaybackInfo& info) {
    if (!fontLoaded) return;

    if (info.title.length() > 50) txtTitle.setString(info.title.substr(0, 47) + L"...");
    else txtTitle.setString(info.title);

    if (info.isPlaying) {
        txtDuration.setString(info.timeText);
    }
    else {
        txtDuration.setString("--:-- / --:--");
    }
}

void InfoPanel::Draw(sf::RenderTarget& target) {
    if (fontLoaded) {
        target.draw(txtTitle);
        target.draw(txtDuration);
    }
}