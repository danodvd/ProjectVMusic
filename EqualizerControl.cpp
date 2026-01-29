#include "EqualizerControl.hpp"
#include <iostream>
#include <algorithm> 

EqualizerControl::EqualizerControl(sf::Vector2f pos) {
    isOpen = false;
    draggingBand = -1;

   if (!font.loadFromFile("arial.ttf")) {
    }

    iconHitbox.setSize({ 30.f, 30.f });
    iconHitbox.setOrigin(15.f, 15.f);
    iconHitbox.setPosition(pos);
    iconHitbox.setFillColor(sf::Color::Transparent); 

    float barW = 6.f;
    float spacing = 2.f;
    float startX = pos.x - 10.f;

    float heights[3] = { 12.f, 20.f, 15.f };

    for (int i = 0; i < 3; i++) {
        iconBars[i].setSize({ barW, heights[i] });
        iconBars[i].setOrigin(barW / 2.f, heights[i] / 2.f); 
        iconBars[i].setPosition(startX + (i * (barW + spacing)), pos.y);
        iconBars[i].setFillColor(sf::Color::White);
    }

    float panelX = pos.x - (PANEL_WIDTH / 2.f);
    float panelY = pos.y - (30.f / 2.f) - PANEL_HEIGHT - 10.f;

    panelBackground.setPosition(panelX, panelY);
    panelBackground.setSize({ PANEL_WIDTH, PANEL_HEIGHT });
    panelBackground.setFillColor(sf::Color(30, 30, 30, 230));
    panelBackground.setOutlineColor(sf::Color(100, 100, 100));
    panelBackground.setOutlineThickness(1.f);

    const char* labels[BANDS] = { "80Hz", "250Hz", "1kHz", "4kHz", "8kHz"};
    float sectionWidth = PANEL_WIDTH / float(BANDS);

    for (int i = 0; i < BANDS; i++) {
        float centerX = panelX + (sectionWidth * i) + (sectionWidth / 2.f);

        float trackH = PANEL_HEIGHT - 40.f;
        sliders[i].track.setSize({ 4.f, trackH });
        sliders[i].track.setOrigin(2.f, 0.f);
        sliders[i].track.setPosition(centerX, panelY + 20.f);
        sliders[i].track.setFillColor(sf::Color(100, 100, 100));

        sliders[i].knob.setRadius(7.f);
        sliders[i].knob.setOrigin(7.f, 7.f);
        sliders[i].knob.setFillColor(sf::Color::Cyan); 
        sliders[i].knob.setPosition(centerX, panelY + 20.f + (trackH / 2.f));

        sliders[i].label.setFont(font);
        sliders[i].label.setString(labels[i]);
        sliders[i].label.setCharacterSize(12);
        sliders[i].label.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = sliders[i].label.getLocalBounds();
        sliders[i].label.setOrigin(textBounds.width / 2.f, 0);
        sliders[i].label.setPosition(centerX, panelY + 5.f);
    }
}

void EqualizerControl::SetPosition(sf::Vector2f newPos) {
    iconHitbox.setPosition(newPos);

    float barW = 6.f;
    float spacing = 2.f;
    float startX = newPos.x - 10.f;

    for (int i = 0; i < BANDS; i++) {
        iconBars[i].setPosition(startX + (i * (barW + spacing)), newPos.y);
    }

    float panelX = newPos.x - (PANEL_WIDTH / 2.f);
    float panelY = newPos.y - (30.f / 2.f) - PANEL_HEIGHT - 10.f;

    panelBackground.setPosition(panelX, panelY);

    float sectionWidth = PANEL_WIDTH / 5.f;
    for (int i = 0; i < 5; i++) {
        float centerX = panelX + (sectionWidth * i) + (sectionWidth / 2.f);

        float oldTrackY = sliders[i].track.getPosition().y;
        float newTrackY = panelY + 20.f;
        float deltaY = newTrackY - oldTrackY;

        sliders[i].track.setPosition(centerX, newTrackY);
        sliders[i].label.setPosition(centerX, panelY + 5.f);

        sf::Vector2f oldKnobPos = sliders[i].knob.getPosition();
        sliders[i].knob.setPosition(centerX, oldKnobPos.y + deltaY);
    }
}

void EqualizerControl::UpdateKnobPosition(int bandIndex, float gain) {
    float y = GainToY(gain, sliders[bandIndex].track.getPosition().y, sliders[bandIndex].track.getSize().y);

    float trackX = sliders[bandIndex].track.getPosition().x;

    sliders[bandIndex].knob.setPosition(trackX, y);
}

void EqualizerControl::SyncWithAudio(AudioManager& audio) {
    for (int i = 0; i < BANDS; i++) {
        float currentGain = audio.GetEQ(i);
        UpdateKnobPosition(i, currentGain);
    }
}

void EqualizerControl::Draw(sf::RenderTarget& target) const {
    for (const auto& bar : iconBars) target.draw(bar);

    if (isOpen) {
        target.draw(panelBackground);
        for (const auto& s : sliders) {
            target.draw(s.label);
            target.draw(s.track);
            target.draw(s.knob);
        }
    }
}

bool EqualizerControl::HandleEvent(const sf::Event& evt, sf::RenderWindow& win, AudioManager& audio) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(win);
    sf::Vector2f mousePos = win.mapPixelToCoords(pixelPos);

    if (evt.type == sf::Event::MouseButtonPressed && evt.mouseButton.button == sf::Mouse::Left) {
        if (iconHitbox.getGlobalBounds().contains(mousePos)) {
            isOpen = !isOpen;
            if (isOpen) SyncWithAudio(audio);
            return true;
        }

        if (isOpen && panelBackground.getGlobalBounds().contains(mousePos)) {
            for (int i = 0; i < BANDS; i++) {
                sf::FloatRect sliderArea = sliders[i].track.getGlobalBounds();
                sliderArea.left -= 10.f;
                sliderArea.width += 20.f;

                if (sliderArea.contains(mousePos)) {
                    draggingBand = i;

                    float newGain = YToGain(mousePos.y, sliders[i].track.getPosition().y, sliders[i].track.getSize().y);
                    audio.SetEQ(i, newGain);
                    UpdateKnobPosition(i, newGain);
                    return true;
                }
            }
            return true;
        }

        if (isOpen && !panelBackground.getGlobalBounds().contains(mousePos)) {
            isOpen = false;
        }
    }
    else if (evt.type == sf::Event::MouseButtonReleased && evt.mouseButton.button == sf::Mouse::Left) {
        draggingBand = -1;
    }
    else if (evt.type == sf::Event::MouseMoved) {
        if (isOpen && draggingBand != -1) {
            float newGain = YToGain(mousePos.y, sliders[draggingBand].track.getPosition().y, sliders[draggingBand].track.getSize().y);

            audio.SetEQ(draggingBand, newGain);

            UpdateKnobPosition(draggingBand, newGain);
            return true;
        }
    }

    return false;
}


float EqualizerControl::GainToY(float gain, float trackTop, float trackHeight) const {
    float normalized = (gain - MIN_DB) / (MAX_DB - MIN_DB);
    return trackTop + trackHeight * (1.0f - normalized);
}

float EqualizerControl::YToGain(float y, float trackTop, float trackHeight) const {
    float relativeY = y - trackTop;
    float normalized = 1.0f - (relativeY / trackHeight); 
    normalized = std::clamp(normalized, 0.0f, 1.0f);

    return MIN_DB + normalized * (MAX_DB - MIN_DB);
}