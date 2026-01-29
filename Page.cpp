#include "Page.hpp"

sf::Texture Page::bgdImg;
std::vector<sf::Color> Page::darkTheme;
std::vector<sf::Color> Page::lightTheme;
std::vector<sf::Color>* Page::currentTheme = nullptr;

Page::Page(AudioManager& audioRef) : audio(audioRef) {
    darkTheme.push_back(sf::Color(32, 16, 40));
    darkTheme.push_back(sf::Color(54, 45, 105));

    lightTheme.push_back(sf::Color::White);

    currentTheme = &darkTheme;

    sf::RectangleShape screenSize(sf::Vector2f(1280.f, 720.f));

    commonBackground = SetBackgroundGradient(screenSize, *currentTheme);
    lastRenderedTheme = currentTheme;
}

void Page::DrawBackground(sf::RenderTarget& target) {
    if (currentTheme != lastRenderedTheme) {
        sf::Vector2f size = commonBackground[2].position; 
        sf::RectangleShape rect(size);
        commonBackground = SetBackgroundGradient(rect, *currentTheme);
        lastRenderedTheme = currentTheme;
    }
    
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
    commonBackground = SetBackgroundGradient(screenSize, *currentTheme);
}

void Page::ChangeBackgroundGradient(Theme theme) {
    switch (theme)
    {
    case Page::Theme::dark:
        currentTheme = &darkTheme;
        break;
    case Page::Theme::light:
        currentTheme = &lightTheme;
        break;
    default:
        break;
    }

    sf::Vector2f currentSize = commonBackground[2].position;
    sf::RectangleShape rect(currentSize);
    commonBackground = SetBackgroundGradient(rect, *currentTheme);
}

sf::VertexArray Page::SetBackgroundGradient(const sf::RectangleShape& background, std::vector<sf::Color> theme) const {
    sf::VertexArray bgd(sf::Quads, 4);

    sf::Color colorLeft(theme[0]);
    sf::Color colorRight(theme.size() > 1 ? theme[1] : theme[0]);

    float width = background.getSize().x;
    float height = background.getSize().y;

    bgd[0].position = sf::Vector2f(0.0f, 0.0f);
    bgd[0].color = colorLeft;

    bgd[1].position = sf::Vector2f(width, 0.f);
    bgd[1].color = colorRight;

    bgd[2].position = sf::Vector2f(width, height);
    bgd[2].color = colorRight;

    bgd[3].position = sf::Vector2f(0.f, height);
    bgd[3].color = colorLeft;

    return bgd;
}

