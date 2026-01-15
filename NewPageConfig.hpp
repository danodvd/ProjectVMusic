#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "Page.hpp"

/**
* Represents the general configuration window of the application.
* 
* This structure contains all rendering-related data needed to draw
* a config entry.
*/
class PageConfig : public Page {
public:
	PageConfig(AudioManager& audioRef);

	void Draw(sf::RenderTarget& target) override;

	void UpdateLayout(sf::Vector2u newSize) override;

private:
	sf::Text title;
	sf::Text changeBgdTxt;
	sf::RectangleShape changeBgdBox;
};