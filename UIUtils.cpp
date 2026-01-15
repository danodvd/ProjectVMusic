#include "UIUtils.hpp"

namespace UIUtils {
	sf::Font globalFont;

	// Implementación de las funciones de carga
	bool LoadFont(const std::string& path) {
		return globalFont.loadFromFile(path);
	}

	const sf::Font& GetFont() {
		return globalFont;
	}

	void UIUtils::ConfigureBox(sf::RectangleShape& box, sf::Vector2f size, sf::Vector2f position, sf::Color color, sf::Color outlineColor, int outlineThickness) {
		box.setSize(size);
		box.setPosition(position);
		box.setFillColor(color);
		box.setOutlineColor(outlineColor);
		box.setOutlineThickness(outlineThickness);
	}

	void UIUtils::ConfigureLabel(sf::Text& label, unsigned int charSize, sf::Vector2f position, const sf::String& text, sf::Color fillColor) {
		label.setFont(GetFont());
		label.setCharacterSize(charSize);
		label.setPosition(position);
		label.setString(text);
		label.setFillColor(fillColor);
	}

	void UIUtils::SetFont(const sf::Font& font, const std::vector<sf::Text*>& texts) {
		for (sf::Text* text : texts) {
			if (text) {
				text->setFont(font);
			}
		}
	}
}