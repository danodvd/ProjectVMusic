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

    // --- CORRECCIÓN DE ALINEACIÓN ---
    // Centramos el origen del sprite para que coincida con el sistema del botón Play
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    // --------------------------------

    // Ajustamos la posición del slider (barra vertical) para tener en cuenta el nuevo origen
    // Como 'pos.y' ahora es el centro del icono, restamos la mitad de la altura (aprox 15px)
    float iconHalfHeight = (bounds.height * 0.4f) / 2.f;

    float sliderHeight = 100.0f;
    float sliderWidth = 30.0f;

    // Slider justo encima del icono
    float posX = pos.x - (sliderWidth / 2.f); // Centrado en X
    float posY = pos.y - iconHalfHeight - sliderHeight - 10; // Arriba en Y

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

    // 1. Detectar inicio/fin del clic
    if (evt.type == sf::Event::MouseButtonPressed && evt.mouseButton.button == sf::Mouse::Left) {
        if (sprite.getGlobalBounds().contains(worldPos)) {
            showSlider = !showSlider;
        }
        // Si clic en el fondo de la barra, iniciar arrastre
        else if (showSlider && volBackground.getGlobalBounds().contains(worldPos)) {
            isDragging = true;
        }
        // Si clic fuera de todo, cerrar slider
        else if (showSlider) {
            showSlider = false;
        }
    }

    if (evt.type == sf::Event::MouseButtonReleased && evt.mouseButton.button == sf::Mouse::Left) {
        isDragging = false;
    }

    VolumeChange result{};

    if (isDragging || (evt.type == sf::Event::MouseButtonPressed && showSlider && volBackground.getGlobalBounds().contains(worldPos))) {

        // Calcular nuevo volumen
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

    // Actualizar posiciones
    volBackground.setPosition(sliderX, sliderY);

    float currentBarHeight = volBar.getSize().y;
    volBar.setPosition(sliderX + sliderWidth / 2, sliderY + (sliderHeight - currentBarHeight) / 2.f);

    volIndicator.setPosition(sliderX + sliderWidth / 2, sliderY + (sliderHeight - currentBarHeight) - 5.f);
}