#include "Page.hpp"

sf::Texture Page::bgdImg;

Page::Page(AudioManager& audioRef) : audio(audioRef) {
    // Crear un rectángulo imaginario del tamaño de la ventana para generar el gradiente
    sf::RectangleShape screenSize(sf::Vector2f(1280.f, 720.f));

    // Guardamos el gradiente en la variable de la clase
    commonBackground = SetBackgroundGradient(screenSize);
}

void Page::DrawBackground(sf::RenderTarget& target) {
    if (BgdImgExists()) {
        sf::Sprite sprite(bgdImg);

        sf::Vector2u imgSize = bgdImg.getSize();
        sf::Vector2u windowSize = target.getSize();

        float scaleX = (float)windowSize.x / imgSize.x;
        float scaleY = (float)windowSize.y / imgSize.y;

        sprite.setScale(scaleX, scaleY);

        target.draw(sprite);
    }
    else {
        target.draw(commonBackground);
    }
}

void Page::ChangeBgdImg(const sf::Texture& newTexture) {
    bgdImg = newTexture;

    bgdImg.setSmooth(true);
}

bool Page::BgdImgExists() {
    return bgdImg.getSize().x != 0 && bgdImg.getSize().y != 0;
}

void Page::HandleClick(const sf::Vector2f& mousePos) {
	for (const auto& btn : buttons) {
		if (btn.bounds.contains(mousePos)) {
			btn.command.get()->Execute();
			return;
		}
	}
}

void Page::UpdateLayout(sf::Vector2u newSize) {
    sf::RectangleShape screenSize(sf::Vector2f((float)newSize.x, (float)newSize.y));
    commonBackground = SetBackgroundGradient(screenSize);
}

sf::VertexArray Page::SetBackgroundGradient(const sf::RectangleShape& background) const {
    sf::VertexArray bgd(sf::Quads, 4);

    sf::Color colorLeft(117, 58, 136);
    sf::Color colorRight(174, 50, 111);

    float width = background.getSize().x;
    float height = background.getSize().y;

    // 1. Top-Left
    bgd[0].position = sf::Vector2f(0.0f, 0.0f);
    bgd[0].color = colorLeft;

    // 2. Top-Right
    bgd[1].position = sf::Vector2f(width, 0.f);
    bgd[1].color = colorRight;

    // 3. Bottom-Right
    bgd[2].position = sf::Vector2f(width, height);
    bgd[2].color = colorRight;

    // 4. Bottom-Left
    bgd[3].position = sf::Vector2f(0.f, height);
    bgd[3].color = colorLeft;

    return bgd;
}

