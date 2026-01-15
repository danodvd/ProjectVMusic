#include "NewPageConfig.hpp"
#include "UIUtils.hpp"
#include "ChangeBackgroundCommand.hpp"

PageConfig::PageConfig(AudioManager& audioRef) : Page(audioRef) {
	UIUtils::ConfigureLabel(title, 36, { 80, 20 }, "Configuración");
	UIUtils::ConfigureLabel(changeBgdTxt, 18, { 100, 100 }, "Cambiar fondo de pantalla");
	UIUtils::ConfigureBox(changeBgdBox, { 250, 50 }, { changeBgdTxt.getPosition().x - 20, changeBgdTxt.getPosition().y - 10 }, sf::Color(255, 255, 255, 40), sf::Color::White);

	Button btnChangeBackground;
	btnChangeBackground.bounds = changeBgdBox.getGlobalBounds();
	btnChangeBackground.command = std::make_unique<ChangeBackgroundCommand>(*this, GetActiveWindow());
	buttons.push_back(std::move(btnChangeBackground));
}

void PageConfig::Draw(sf::RenderTarget& target) {
	Page::DrawBackground(target);
	target.draw(title);
	target.draw(changeBgdTxt);
	target.draw(changeBgdBox);
}

void PageConfig::UpdateLayout(sf::Vector2u newSize) {
	Page::UpdateLayout(newSize);

	float w = (float)newSize.x;
	float h = (float)newSize.y;

	title.setPosition(80, 20);

	float textX = (w / 2.f) - (changeBgdTxt.getGlobalBounds().width / 2.f);
	float textY = (h / 2.f) - 50.f;
	changeBgdTxt.setPosition(textX, textY);

	changeBgdBox.setPosition(textX - 20, textY - 10);

	if (!buttons.empty()) buttons[0].bounds = changeBgdBox.getGlobalBounds();
}