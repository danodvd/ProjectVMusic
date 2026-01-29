#pragma once 
#include "SFML/Graphics.hpp"
#include "NewAudioManager.hpp"

class EqualizerControl {
public:
    EqualizerControl(sf::Vector2f pos);

    void Draw(sf::RenderTarget& target) const;

    bool HandleEvent(const sf::Event& evt, sf::RenderWindow& win, AudioManager& audio);

    void SetPosition(sf::Vector2f newPos);

    void SyncWithAudio(AudioManager& audio);

private:
    static constexpr int BANDS = 5;
    sf::RectangleShape iconHitbox; 
    std::array<sf::RectangleShape, BANDS> iconBars;

    sf::RectangleShape panelBackground;

    struct Slider {
        sf::RectangleShape track;     
        sf::CircleShape knob;         
        sf::Text label;              
    };
    std::array<Slider, BANDS> sliders;
    sf::Font font;

    bool isOpen;
    int draggingBand; 

    const float MIN_DB = -15.0f;
    const float MAX_DB = 15.0f;
    const float PANEL_WIDTH = 200.0f;
    const float PANEL_HEIGHT = 150.0f;

    float GainToY(float gain, float trackTop, float trackHeight) const;
    float YToGain(float y, float trackTop, float trackHeight) const;
    void UpdateKnobPosition(int bandIndex, float gain);
};