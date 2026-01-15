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
    background.setFillColor(sf::Color(30, 30, 30, 250));
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

    UIUtils::ConfigureLabel(txtMain, 18, { TEXTS_POS_X, 80.f }, "Página Principal");
    UIUtils::ConfigureLabel(txtLibrary, 18, { TEXTS_POS_X, 130.f }, "Mi Música");
    UIUtils::ConfigureLabel(txtQueue, 18, { TEXTS_POS_X, 180.f }, "Cola de Reproducción");
    UIUtils::ConfigureLabel(txtConf, 18, { TEXTS_POS_X, background.getSize().y - 40 }, "Configuración");

    UIUtils::ConfigureBox(boxMain, { BOXES_SIZE_X, BOXES_SIZE_Y }, txtMain.getPosition() - sf::Vector2f(BOXES_OFFSET_X, BOXES_OFFSET_Y), {}, sf::Color::White);
    areaMain = boxMain.getGlobalBounds();

    UIUtils::ConfigureBox(boxLibrary, { BOXES_SIZE_X, BOXES_SIZE_Y }, txtLibrary.getPosition() - sf::Vector2f(BOXES_OFFSET_X, BOXES_OFFSET_Y), {}, sf::Color::White);
    areaLibrary = boxLibrary.getGlobalBounds();

    UIUtils::ConfigureBox(boxQueue, { BOXES_SIZE_X, BOXES_SIZE_Y }, txtQueue.getPosition() - sf::Vector2f(BOXES_OFFSET_X, BOXES_OFFSET_Y), {}, sf::Color::White);
    areaQueue = boxQueue.getGlobalBounds();

    UIUtils::ConfigureBox(boxConf, { BOXES_SIZE_X, BOXES_SIZE_Y }, txtConf.getPosition() - sf::Vector2f(BOXES_OFFSET_X, BOXES_OFFSET_Y), {}, sf::Color::White);
    areaConf = boxConf.getGlobalBounds();

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

void NavBar::SetHeight(float height) {
    // Asumiendo que 'navBackground' es el rectángulo vertical
    background.setSize({ background.getSize().x, height });
}

