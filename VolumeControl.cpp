#include "VolumeControl.hpp"

void VolumeControl::LoadFont(std::string fontPath) {
    if (font.loadFromFile(fontPath)) {
        fontLoaded = true;

        UIUtils::SetFont(font, { &txtVolume });
        UIUtils::ConfigureLabel(txtVolume, 20, { 20, 450 });
    }
}

VolumeControl::VolumeControl(sf::Vector2f pos) {
    fontLoaded = false;
    std::stringstream ss;

    showSlider = false;
    isDragging = false;

    txtVolume.setString(ss.str());

    texMute = VolumeIcons::Mute();
    texLow = VolumeIcons::Volume(1);
    texMid = VolumeIcons::Volume(2);
    texHigh = VolumeIcons::Volume(3);

    sprite.setPosition(pos.x, pos.y);
    sprite.setTexture(texHigh);
    sprite.setScale(0.4f, 0.4f);

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    float iconHalfHeight = (bounds.height * 0.4f) / 2.f;

    float sliderHeight = 100.0f;
    float sliderWidth = 30.0f;

    float posX = pos.x - (sliderWidth / 2.f); 
    float posY = pos.y - iconHalfHeight - sliderHeight - 10;

    UIUtils::ConfigureBox(volBackground, { sliderWidth, sliderHeight }, { posX, posY }, sf::Color(30, 30, 30, 230), sf::Color::White);

    UIUtils::ConfigureBox(volBackground, { sliderWidth, sliderHeight }, { posX, posY }, sf::Color(30, 30, 30, 230), sf::Color::White);

    UIUtils::ConfigureBox(volBar, { 4, sliderHeight - 20 }, { posX + sliderWidth / 2, posY + 10 }, sf::Color(100, 100, 100));
    volBar.setOrigin(2, 0);

    UIUtils::ConfigureBox(volIndicator, { 16, 8 }, { posX + sliderWidth / 2, posY + sliderHeight - 10 }, sf::Color::Cyan);
    volIndicator.setOrigin(8, 4);
}

void VolumeControl::Update(float vol) {
    if (vol <= 0.0f) sprite.setTexture(texMute);
    else if (vol <= 0.33f) sprite.setTexture(texLow);
    else if (vol <= 0.66f) sprite.setTexture(texMid);
    else sprite.setTexture(texHigh);

    float barTop = volBar.getPosition().y;
    float barHeight = volBar.getSize().y;

    float newY = barTop + barHeight * (1.0f - vol);

    volIndicator.setPosition(volIndicator.getPosition().x, newY);
}

void VolumeControl::Draw(sf::RenderTarget& target) const {
    target.draw(sprite);

    if (showSlider) {
        target.draw(volBackground);
        target.draw(volBar);
        target.draw(volIndicator);
    }
}

float VolumeControl::ComputeVolumeFromMouse(float mouseY) const {
    float top = volBar.getPosition().y;
    float bottom = top + volBar.getSize().y;
    mouseY = std::clamp(mouseY, top, bottom);
    return (bottom - mouseY) / volBar.getSize().y;
}


VolumeChange VolumeControl::HandleEvent(const sf::Event& evt, sf::RenderWindow& win) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(win);
    sf::Vector2f worldPos = win.mapPixelToCoords(pixelPos);

    if (evt.type == sf::Event::MouseButtonPressed && evt.mouseButton.button == sf::Mouse::Left) {
        if (sprite.getGlobalBounds().contains(worldPos)) {
            showSlider = !showSlider;
        }
        else if (showSlider && volBackground.getGlobalBounds().contains(worldPos)) {
            isDragging = true;
        }
        else if (showSlider) {
            showSlider = false;
        }
    }

    if (evt.type == sf::Event::MouseButtonReleased && evt.mouseButton.button == sf::Mouse::Left) {
        isDragging = false;
    }

    VolumeChange result{};

    if (isDragging || (evt.type == sf::Event::MouseButtonPressed && showSlider && volBackground.getGlobalBounds().contains(worldPos))) {

        result.changed = true;
        result.value = ComputeVolumeFromMouse(worldPos.y);
    }

    return result;
}

void VolumeControl::SetPosition(sf::Vector2f newPos) {
    sprite.setPosition(newPos);

    sf::FloatRect bounds = sprite.getLocalBounds();
    float iconHalfHeight = (bounds.height * 0.4f) / 2.f;
    float sliderHeight = 100.0f;
    float sliderWidth = 30.0f;

    float sliderX = newPos.x - (sliderWidth / 2.f);
    float sliderY = newPos.y - iconHalfHeight - sliderHeight - 10.f;

    volBackground.setPosition(sliderX, sliderY);

    float currentBarHeight = volBar.getSize().y;
    volBar.setPosition(sliderX + sliderWidth / 2, sliderY + (sliderHeight - currentBarHeight) / 2.f);

    volIndicator.setPosition(sliderX + sliderWidth / 2, sliderY + (sliderHeight - currentBarHeight) - 5.f);
}