#pragma once
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "UIUtils.hpp"

class NavBar {
public:

    enum class ClickTarget {
        None,
        Main,
        Library,
        Queue,
        Config,
        ToggleNav
    };


    NavBar();

    sf::FloatRect GetFullSize();

    void Update(bool isOpen);

    void Draw(sf::RenderTarget& target) const;

    bool ContainsToggle(sf::Vector2f mousePos) const;

    ClickTarget HandleClick(sf::Vector2f mousePos) const;

    bool Contains(const sf::Vector2f& mousePos);

    void SetHeight(float height);
private:
    bool open = false;

    sf::FloatRect areaMusic;
    sf::FloatRect areaMain;
    sf::FloatRect areaLibrary;
    sf::FloatRect areaQueue;
    sf::FloatRect areaConf;

    std::vector<sf::RectangleShape> bars;
    sf::RectangleShape background;

    sf::RectangleShape boxMain;
    sf::RectangleShape boxLibrary;
    sf::RectangleShape boxQueue;
    sf::RectangleShape boxConf;

    sf::Text txtMain;
    sf::Text txtLibrary;
    sf::Text txtQueue;
    sf::Text txtConf;

    float baseX = 20.f;
    float baseY = 20.f;

    static constexpr float TEXTS_POS_X = 20.f;

    static constexpr float BOXES_SIZE_X = 250.f;
    static constexpr float BOXES_SIZE_Y = 40.f;

    static constexpr float BOXES_OFFSET_X = 25.f;
    static constexpr float BOXES_OFFSET_Y = 10.f;
  
    static constexpr float BAR_WIDTH = 30.f;
    static constexpr float BAR_HEIGHT = 5.f;
    static constexpr float BAR_SPACING = 10.f;
};