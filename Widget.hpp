#include "SFML/Graphics.hpp"

class Widget {
public:
	virtual void SetPosition(sf::Vector2f pos);

	virtual void SetSize(sf::Vector2f size);

	virtual void Update();
private:
};