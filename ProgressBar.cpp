#include "ProgressBar.hpp"

ProgressBar::ProgressBar(sf::Vector2f pos, sf::Vector2f size) {
    background.setSize(size);
    background.setPosition(pos);
    background.setFillColor(sf::Color(50, 50, 50));

    fill.setSize({ 0.f, size.y });
    fill.setPosition(pos);
    fill.setFillColor(sf::Color(0, 255, 255));
}

void ProgressBar::Update(const PlaybackInfo& info) {
    if (!info.isPlaying || info.len <= 0) {
        fill.setSize({ 0.f, background.getSize().y });
        return;
    }

    double pct = info.pos / info.len;
    float width = background.getSize().x;

    fill.setSize({ width * float(pct), background.getSize().y });
}

void ProgressBar::Draw(sf::RenderTarget& target) const {
    target.draw(background); 
    target.draw(fill);
}

bool ProgressBar::TrySeek(sf::Vector2f mousePos, double& outPct) const {
    if (!background.getGlobalBounds().contains(mousePos)) return false;

    outPct = std::clamp(
        (mousePos.x - background.getPosition().x) / background.getSize().x,
        0.0f, 1.0f
    );
    return true;
}

void ProgressBar::SetPosition(sf::Vector2f newPos) {
    background.setPosition(newPos);
    fill.setPosition(newPos);
}

void ProgressBar::SetSize(sf::Vector2f size) {
    background.setSize(size);
}