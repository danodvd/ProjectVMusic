#include "VolumeIcons.hpp"

sf::Texture VolumeIcons::Speaker() {
    sf::RenderTexture rt;
    rt.create(128, 128);
    rt.clear(sf::Color::Transparent);

    // Speaker
    sf::RectangleShape box(sf::Vector2f(14, 26));
    box.setPosition(30, 51);
    box.setFillColor(sf::Color::White);
    rt.draw(box);

    sf::ConvexShape cone;
    cone.setPointCount(4);

    cone.setPoint(0, { 44, 51 });
    cone.setPoint(1, { 65, 35 });
    cone.setPoint(2, { 65, 93 });
    cone.setPoint(3, { 44, 77 });

    cone.setFillColor(sf::Color::White);
    rt.draw(cone);

    rt.display();
    return rt.getTexture();
}

sf::Texture VolumeIcons::Volume(int level) {
    sf::RenderTexture rt;
    rt.create(128, 128);
    rt.clear(sf::Color::Transparent);

    sf::Texture baseTexture = Speaker();
    sf::Sprite speakerSprite(baseTexture);

    rt.draw(speakerSprite);

    // Waves

    sf::Vector2f center(50.f, 64.f);

    float thickness = 6.0f;
    float pi = 3.14159265f;
    float degreeStep = 4.0f;

    auto drawThickArc = [&](float radius) {
        for (float angle = -45.0f; angle <= 45.0f; angle += degreeStep) {
            float rad = angle * (pi / 180.0f);

            float x = center.x + std::cos(rad) * radius + 15;
            float y = center.y + std::sin(rad) * radius;

            sf::RectangleShape seg(sf::Vector2f(thickness, thickness));
            seg.setOrigin(thickness / 2, thickness / 2);
            seg.setPosition(x, y);
            seg.setRotation(angle);
            seg.setFillColor(sf::Color::White);
            rt.draw(seg);
        }
    };

    if (level >= 1) drawThickArc(15.0f);
    if (level >= 2) drawThickArc(25.0f);
    if (level >= 3) drawThickArc(35.0f);


    rt.display();
    return rt.getTexture();
}

sf::Texture VolumeIcons::Mute() {
    sf::RenderTexture rt;
    rt.create(128, 128);
    rt.clear(sf::Color::Transparent);

    sf::Texture baseTexture = Speaker();
    sf::Sprite speakerSprite(baseTexture);

    rt.draw(speakerSprite);

    sf::RectangleShape line({26.f, 7.f});
    line.setOrigin(13.f, 3.5f);
    line.setPosition(85.f, 64.f);
    line.setFillColor(sf::Color::White);

    line.setRotation(45.f);
    rt.draw(line);

    line.setRotation(-45.f);
    rt.draw(line);

    rt.display();
    return rt.getTexture();
}
