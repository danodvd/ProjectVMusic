#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/ConvexShape.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include <iomanip>

class VolumeIcons {
public:
    static sf::Texture Speaker();
    static sf::Texture Volume(int level);
    static sf::Texture Mute();
};