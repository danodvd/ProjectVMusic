#pragma once
#include "SFML/Graphics.hpp"
#include "NewAudioManager.hpp"

class SubView {
public:
	virtual void Draw(sf::RenderTarget& target) = 0;
	virtual void HandleClick(const sf::Vector2f& mousePos) = 0;

	virtual void SetActive() = 0;
};