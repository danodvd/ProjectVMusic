#include "ButtonNav.hpp"

static sf::FloatRect UnionRect(const sf::FloatRect& a, const sf::FloatRect& b) {
    float left = std::min(a.left, b.left);
    float top = std::min(a.top, b.top);
    float right = std::max(a.left + a.width, b.left + b.width);
    float bottom = std::max(a.top + a.height, b.top + b.height);
    return sf::FloatRect(left, top, right - left, bottom - top);
}

NavButton::NavButton(const sf::Vector2f& pos, Direction dir) {
    // Inicializamos en la posición dada (esto llama a nuestra lógica centrada)
    SetPosition(pos, dir);
}

void NavButton::SetPosition(sf::Vector2f centerPos, Direction dir) {
    // --- DIMENSIONES ---
    float size = 20.f;          // Escala base
    float height = size * 2;    // 40px
    float arrowW = size + 5;    // 25px
    float barW = 4.f;           // 4px
    float spacing = 0.f;        // Espacio entre flecha y barra

    float totalWidth = arrowW + spacing + barW; // ~35px total

    // --- CÁLCULO DE ORIGEN (Top-Left) BASADO EN EL CENTRO ---
    // Si centerPos es el centro, restamos la mitad del ancho y alto para hallar la esquina
    float startX = centerPos.x - (totalWidth / 2.f);
    float startY = centerPos.y - (height / 2.f);

    // 1. Configurar Puntos del Triángulo (Arrow)
    arrow.setPointCount(3);
    arrow.setFillColor(sf::Color::White);

    if (dir == Direction::Next) {
        // [Flecha] [Espacio] [Barra]  (>|)

        // Flecha
        arrow.setPoint(0, { 0, 0 });
        arrow.setPoint(1, { 0, height });
        arrow.setPoint(2, { arrowW, height / 2.f }); // Punta
        arrow.setPosition(startX, startY);

        // Barra
        bar.setSize({ barW, height });
        bar.setPosition(startX + arrowW + spacing, startY);
    }
    else {
        // [Barra] [Espacio] [Flecha]  (|<)

        // Barra
        bar.setSize({ barW, height });
        bar.setPosition(startX, startY);

        // Flecha (apunta izquierda)
        arrow.setPoint(0, { arrowW, 0 });
        arrow.setPoint(1, { arrowW, height });
        arrow.setPoint(2, { 0, height / 2.f }); // Punta
        arrow.setPosition(startX + barW + spacing, startY);
    }

    bar.setFillColor(sf::Color::White);

    // 2. ACTUALIZAR ZONA DE CLIC (Centrada)
    // Hacemos el área un poco más grande (padding) para facilitar el clic
    bounds = sf::FloatRect(startX - 5, startY - 5, totalWidth + 10, height + 10);
}

void NavButton::Draw(sf::RenderTarget& target) const {
    target.draw(arrow);
    target.draw(bar);
}

bool NavButton::Contains(const sf::Vector2f& mouse) const {
    return bounds.contains(mouse);
}
