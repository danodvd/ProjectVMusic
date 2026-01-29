#include "NewPageConfig.hpp"
#include "UIUtils.hpp"
#include "ChangeBackgroundCommand.hpp"
#include "ChangeThemeCommand.hpp"

PageConfig::PageConfig(AudioManager& audioRef) : Page(audioRef) {
    UIUtils::ConfigureLabel(title, 36, { 0, 0 }, "Configuración"); 
    UIUtils::ConfigureLabel(changeBgdTxt, 18, { 0, 0 }, "Cambiar fondo de pantalla");
    UIUtils::ConfigureLabel(changeBgdThemeTxt, 18, { 0, 0 }, "Cambiar Tema de la Aplicación");
    UIUtils::ConfigureLabel(lightThemeTxt, 18, { 0, 0 }, "Claro");
    UIUtils::ConfigureLabel(darkThemeTxt, 18, { 0, 0 }, "Oscuro");

    UIUtils::ConfigureBox(changeBgdBox, { 250, 50 }, { 0, 0 }, sf::Color(255, 255, 255, 40), sf::Color::White);
    UIUtils::ConfigureBox(changeBgdThemeBox, { 300, 50 }, { 0, 0 }, sf::Color(255, 255, 255, 40), sf::Color::White);
    UIUtils::ConfigureBox(changeBgdThemeDropdown, { 300, 100 }, { 0, 0 }, sf::Color(255, 255, 255, 40), sf::Color(40, 40, 40, 40)); // Hice el dropdown del mismo ancho que la caja de arriba
    UIUtils::ConfigureBox(lightThemeBox, { 280, 40 }, { 0, 0 }, sf::Color(255, 255, 255, 40), sf::Color::White);
    UIUtils::ConfigureBox(darkThemeBox, { 280, 40 }, { 0, 0 }, sf::Color(255, 255, 255, 40), sf::Color::White);

    Button btnChangeBackground;
    btnChangeBackground.command = std::make_unique<ChangeBackgroundCommand>(*this, GetActiveWindow());
    buttons.push_back(std::move(btnChangeBackground));

    Button btnChangeThemeLight;
    btnChangeThemeLight.command = std::make_unique<ChangeThemeCommand>(*this, Page::Theme::light);
    buttons.push_back(std::move(btnChangeThemeLight));

    Button btnChangeThemeDark;
    btnChangeThemeDark.command = std::make_unique<ChangeThemeCommand>(*this, Page::Theme::dark);
    buttons.push_back(std::move(btnChangeThemeDark));

}

void PageConfig::Draw(sf::RenderTarget& target) {
    Page::DrawBackground(target);
    target.draw(title);

    target.draw(changeBgdBox);
    target.draw(changeBgdTxt);

    target.draw(changeBgdThemeBox);
    target.draw(changeBgdThemeTxt);

    if (enableThemeDropdown) {
        target.draw(changeBgdThemeDropdown);
        target.draw(lightThemeBox);
        target.draw(lightThemeTxt);
        target.draw(darkThemeBox);
        target.draw(darkThemeTxt);
    }
}

void CenterTextInBox(sf::Text& text, const sf::RectangleShape& box) {
    sf::FloatRect textBounds = text.getLocalBounds(); 
    sf::Vector2f boxPos = box.getPosition();
    sf::Vector2f boxSize = box.getSize();

    float x = boxPos.x + (boxSize.x / 2.0f) - (textBounds.width / 2.0f) - textBounds.left;
    float y = boxPos.y + (boxSize.y / 2.0f) - (textBounds.height / 2.0f) - textBounds.top;

    text.setPosition(x, y);
}

void PageConfig::UpdateLayout(sf::Vector2u newSize) {
    Page::UpdateLayout(newSize);

    float w = (float)newSize.x;
    float h = (float)newSize.y;
    float centerX = w / 2.0f;

    title.setPosition(80, 20);

    sf::Vector2f boxSize = changeBgdBox.getSize();
    changeBgdBox.setPosition(centerX - (boxSize.x / 2.0f), h * 0.3f);
    CenterTextInBox(changeBgdTxt, changeBgdBox);

    sf::Vector2f themeBoxSize = changeBgdThemeBox.getSize();
    changeBgdThemeBox.setPosition(centerX - (themeBoxSize.x / 2.0f), changeBgdBox.getPosition().y + 80.f);
    CenterTextInBox(changeBgdThemeTxt, changeBgdThemeBox);

    changeBgdThemeDropdown.setPosition(changeBgdThemeBox.getPosition().x, changeBgdThemeBox.getPosition().y + themeBoxSize.y);

    float dropdownX = changeBgdThemeDropdown.getPosition().x;
    float dropdownY = changeBgdThemeDropdown.getPosition().y;

    lightThemeBox.setPosition(dropdownX + 10, dropdownY + 10);
    CenterTextInBox(lightThemeTxt, lightThemeBox);

    darkThemeBox.setPosition(dropdownX + 10, lightThemeBox.getPosition().y + lightThemeBox.getSize().y + 10);
    CenterTextInBox(darkThemeTxt, darkThemeBox);

    if (buttons.size() >= 3) {
        buttons[0].bounds = changeBgdBox.getGlobalBounds();
        buttons[1].bounds = lightThemeBox.getGlobalBounds();
        buttons[2].bounds = darkThemeBox.getGlobalBounds();
    }
}

void PageConfig::HandleClick(const sf::Vector2f& mousePos) {
    if (changeBgdThemeBox.getGlobalBounds().contains(mousePos)) {
        enableThemeDropdown = !enableThemeDropdown;
    }

    if (!enableThemeDropdown) {
        if (buttons[0].bounds.contains(mousePos) && buttons[0].command) {
            buttons[0].command->Execute();
        }
    }
    else {
        Page::HandleClick(mousePos);
    }
}