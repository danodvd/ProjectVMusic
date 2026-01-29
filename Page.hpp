#pragma once
#include "SFML/Graphics.hpp"
#include <memory>
#include <vector>
#include "Command.hpp"
#include "NewAudioManager.hpp"
#include "PlayBackInfo.hpp"

/**
* Represents a clickable UI button.
* 
* A button defines a rectangular clickable area and an associated command
* that is executed when the button is clicked.
*/
struct Button {
    /// Clickable area in world coordinates.
    sf::FloatRect bounds;

    /// Command executed when the button is clicked.
    std::unique_ptr<Command> command;
};

/**
* Base class for UI pages/screens.
* 
* A Page contains a collection of clickable buttons and provides common
* input handling logic for mouse interactions.
* Derived classes are responsible for defining the buttons and their layout.
*/
class Page {
public:
    enum class Theme {
        dark,
        light
    };
    Page(AudioManager& audioRef);

    virtual ~Page() = default;

    /**
    * Handles a mouse click in world coordinates
    * 
    * Detects whether the click intersects any button and executes the
    * associated command
    * 
    * @param mousePos Mouse position in world coordinates.
    */
    virtual void HandleClick(const sf::Vector2f& mousePos);

    /**
    * Draws the object to the given render target.
    * 
    * The object must be fully configured before calling this method.
    * 
    * @param target The target where the object will be drawn.
    */
    virtual void Draw(sf::RenderTarget& target) = 0;

    virtual void Update(const PlaybackInfo& info) {}
    virtual void Update() {}

    virtual void UpdateLayout(sf::Vector2u newSize);

    void DrawBackground(sf::RenderTarget& target);

    void ChangeBgdImg(const sf::Texture& bgdImg);

    bool BgdImgExists();

    void ChangeBackgroundGradient(Theme theme);
protected:
    /// Collection of buttons belonging to this page.
    std::vector<Button> buttons;

    AudioManager& audio;

    /**
    * Sets a background gradient color for this page.
    *
    * @param background The background in which the gradient is created.
    */
    sf::VertexArray SetBackgroundGradient(const sf::RectangleShape& background, std::vector<sf::Color> theme) const;

    sf::VertexArray commonBackground;

    static sf::Texture bgdImg;
    static std::vector<sf::Color> darkTheme;
    static std::vector<sf::Color> lightTheme;
    static std::vector<sf::Color>* currentTheme;

    const std::vector<sf::Color>* lastRenderedTheme = nullptr;
};