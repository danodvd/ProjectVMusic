#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

/*Contains backend logic used in all or most files*/

namespace UIUtils {
	// Función estática para cargarla (devuelve true si tuvo éxito)
	bool LoadFont(const std::string& path);
	const sf::Font& GetFont();

	/**
	* Sets the given font for all specified SFML text objects.
	* 
	* The font object must remain valid for the lifetime of the texts.
	* 
	* @param font Font to use
	* @param texts Collection of text objects
	*/
	void SetFont(const sf::Font& font, const std::vector<sf::Text*>& texts);

	/**
	 * Configures an existing SFML rectangle shape.
	 *
	 * The function modifies the given rectangle in-place, setting its size,
	 * position, fill color, and outline properties.
	 * 
	 * @param box Rectangle shape to configure.
	 * @param size Size of the rectangle in pixels (default: 250x100).
	 * @param position Position of the rectangle in world coordinates (default: 0,0).
	 * @param fillColor Fill color of the rectangle (default: white).
	 * @param outlineColor Outline color of the rectangle (default: black).
	 * @param outlineThickness Thickness of the outline in pixels. Use 0 for no outline.
	 */
	void ConfigureBox(sf::RectangleShape& box, 
		sf::Vector2f size = { 250.f, 100.f },
		sf::Vector2f position = { 0.f, 0.f },
		sf::Color color = sf::Color::White, 
		sf::Color outlineColor = sf::Color::Black, 
		int outlineThickness = 1);

	/**
	* Configures an existing SFML text.
	* 
	* The function modifies the given text in-place, setting its character size, position, string and color.
	* The text must have a valid font assigned overhand
	* 
	* @param label Text label to configure.
	* @param charSize Charater size in pixels (default: 18).
	* @param position Position of the label in world coordinates (default: 0, 0).
	* @param text Text string to assign to the label (default: sampleText).
	* @param fillColor Color of the text (default: white).
	*/
	void ConfigureLabel(sf::Text& label,
		unsigned int charSize = 18,
		sf::Vector2f position = { 0.f, 0.f },
		const sf::String& text = "sampleText",
		sf::Color fillColor = sf::Color::White);
};