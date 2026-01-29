#include "NavBar.hpp"
#include <algorithm>

static sf::FloatRect UnionRect(const sf::FloatRect& a, const sf::FloatRect& b) {
    float left = std::min(a.left, b.left);
    float top = std::min(a.top, b.top);
    float right = std::max(a.left + a.width, b.left + b.width);
    float bottom = std::max(a.top + a.height, b.top + b.height);
    return sf::FloatRect(left, top, right - left, bottom - top);
}

NavBar::NavBar()
    : baseX(15), baseY(15)
{
    constexpr int BAR_COUNT = 3;

    bars.resize(BAR_COUNT);

    for (std::size_t i = 0; i < bars.size(); ++i) {
        bars[i].setPosition(
            baseX,
            baseY + i * BAR_SPACING
        );
        bars[i].setSize({BAR_WIDTH, BAR_HEIGHT});
        bars[i].setFillColor(sf::Color::White);
    }

    background.setSize(sf::Vector2f(250, 720));
    background.setOutlineThickness(1);
    background.setOutlineColor(sf::Color(15, 15, 15, 255));
    background.setFillColor(sf::Color(18,4,26));
    background.setPosition(sf::Vector2f(0, 0));
}

sf::FloatRect NavBar::GetFullSize() {
    return background.getGlobalBounds();
}

void NavBar::Update(bool isOpen) {
    open = isOpen;

    float offsetX = open ? background.getSize().x + 5 : 0.f;

    for (std::size_t i = 0; i < bars.size(); ++i) {
        bars[i].setPosition(
            baseX + offsetX,
            baseY + i * BAR_SPACING
        );
    }

    if (!open) return;

    background.setPosition(0.f, 0.f);

    float startY = 70.f;    
    float boxHeight = BOXES_SIZE_Y;
    float gap = 0.f;        

    float textOffsetY = (boxHeight / 2.f) - (18.f / 2.f) - 3.f;

    float currentY = startY;

    UIUtils::ConfigureBox(boxMain, { BOXES_SIZE_X, boxHeight }, { 0.f, currentY }, sf::Color::Transparent, {}, 0);
    areaMain = boxMain.getGlobalBounds();

    UIUtils::ConfigureLabel(txtMain, 18, { 20.f, currentY + textOffsetY }, "Página Principal");

    currentY += boxHeight + gap; 

    UIUtils::ConfigureBox(boxLibrary, { BOXES_SIZE_X, boxHeight }, { 0.f, currentY }, sf::Color::Transparent, {}, 0);
    areaLibrary = boxLibrary.getGlobalBounds();
    UIUtils::ConfigureLabel(txtLibrary, 18, { 20.f, currentY + textOffsetY }, "Mi Música");

    currentY += boxHeight + gap;

    UIUtils::ConfigureBox(boxQueue, { BOXES_SIZE_X, boxHeight }, { 0.f, currentY }, sf::Color::Transparent, {}, 0);
    areaQueue = boxQueue.getGlobalBounds();
    UIUtils::ConfigureLabel(txtQueue, 18, { 20.f, currentY + textOffsetY }, "Cola de Reproducción");

    float confY = background.getSize().y - 50.f;
    UIUtils::ConfigureBox(boxConf, { BOXES_SIZE_X, boxHeight }, { 0.f, confY }, sf::Color::Transparent, {}, 0);
    areaConf = boxConf.getGlobalBounds();
    UIUtils::ConfigureLabel(txtConf, 18, { 20.f, confY + textOffsetY }, "Configuración");
}

void NavBar::Draw(sf::RenderTarget& target) const {
    if (open) {
        target.draw(background);

        target.draw(boxMain);
        target.draw(boxLibrary);
        target.draw(boxQueue);
        target.draw(boxConf);

        target.draw(txtMain);
        target.draw(txtLibrary);
        target.draw(txtQueue);
        target.draw(txtConf);
    }

    for (const auto& bar : bars) target.draw(bar);
}

bool NavBar::Contains(const sf::Vector2f& mousePos) {
    if (open) {
        return background.getGlobalBounds().contains(mousePos);
    }
    else {
        return background.getGlobalBounds().contains(mousePos);
    }
}

bool NavBar::ContainsToggle(sf::Vector2f mousePos) const {
    if (bars.empty()) return false;

    sf::FloatRect area = bars.front().getGlobalBounds();

    for (std::size_t i = 1; i < bars.size(); ++i)
        area = UnionRect(area, bars[i].getGlobalBounds());

    return area.contains(mousePos);
}

NavBar::ClickTarget NavBar::HandleClick(sf::Vector2f mousePos) const {
    if (areaMain.contains(mousePos)) return ClickTarget::Main; 
    if (areaLibrary.contains(mousePos)) return ClickTarget::Library;
    if (areaQueue.contains(mousePos)) return ClickTarget::Queue;
    if (areaConf.contains(mousePos)) return ClickTarget::Config;

    return ClickTarget::None;
}

void NavBar::HandleEvent(sf::Vector2f mousePos, sf::Event events) {
    if (events.type == sf::Event::MouseMoved) {

        sf::Color hoverColor(140, 140, 140, 140);
        sf::Color normalColor = sf::Color::Transparent;


        if (areaMain.contains(mousePos)) boxMain.setFillColor(hoverColor);
        else boxMain.setFillColor(normalColor);

        if (areaLibrary.contains(mousePos)) boxLibrary.setFillColor(hoverColor);
        else boxLibrary.setFillColor(normalColor);

        if (areaQueue.contains(mousePos)) boxQueue.setFillColor(hoverColor);
        else boxQueue.setFillColor(normalColor);

        if (areaConf.contains(mousePos)) boxConf.setFillColor(hoverColor);
        else boxConf.setFillColor(normalColor);
    }
}

void NavBar::SetHeight(float height) {
    background.setSize({ background.getSize().x, height });
}

