#include "Application.hpp"
#include <iostream>

static bool isMenuOpen = false;

Application::Application()
    : window(sf::VideoMode(1280, 720), "Project V Music", sf::Style::Default)
    , progressBar({ 0.f, 0.f }, { 0.f, 0.f })
    , playButton({ 0.f, 0.f })
    , volumeControl({ 0.f, 0.f })
    , navButtonNext({ 0.f, 0.f }, NavButton::Direction::Next)
    , navButtonPrev({ 0.f, 0.f }, NavButton::Direction::Previous)
    , pageManager(audio)
{
    window.setFramerateLimit(60);
    if (!audio.Init()) {
        std::cerr << "Error inicializando Audio Manager" << std::endl;
    }
    currentPage = pageManager.GetCurrentPage();

    footerBackground.setFillColor(sf::Color(20, 20, 20, 230));

    UpdateGlobalLayout(window.getSize());
    pageManager.UpdateAllLayouts(window.getSize());
}

Application::~Application() {
    audio.Free();
}

int Application::Run() {
    while (window.isOpen()) {
        HandleEvents();
        Update();
        Render();
    }
    return 0;
}

void Application::UpdateGlobalLayout(sf::Vector2u size) {
    float w = (float)size.x;
    float h = (float)size.y;

    // --- CONFIGURACIÓN DEL FOOTER ---
    float footerHeight = 80.f;       // Altura de la zona de controles
    float footerY = h - footerHeight; // Donde empieza el footer (Y)

    // 1. Fondo
    footerBackground.setSize({ w, footerHeight });
    footerBackground.setPosition(0, footerY);

    // 2. Barra de Progreso (Justo en el borde superior del footer)
    // La ponemos un poco más arriba (footerY - 5) o justo en el borde (footerY)
    progressBar.SetPosition({ 0.f, footerY - 10.f });
    progressBar.SetSize(sf::Vector2f(w, 10));

    // 3. Botones (Centrados verticalmente en el footer)
    float centerY = footerY + (footerHeight / 2.f);

    // Botón Play
    playButton.SetPosition({ w / 2.f, centerY });

    // Botones Navegación (A los lados del Play)
    navButtonPrev.SetPosition({ (w / 2.f) - 150.f, centerY }, NavButton::Direction::Previous);
    navButtonNext.SetPosition({ (w / 2.f) + 150.f, centerY }, NavButton::Direction::Next);

    // Volumen (A la derecha, centrado)
    volumeControl.SetPosition({ w - 100.f, centerY });

    // Navbar (Ajustar altura para que no tape el footer)
    navBar.SetHeight(footerY - 10);
}

void Application::HandleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, (float)event.size.width, (float)event.size.height);
            window.setView(sf::View(visibleArea));

            sf::Vector2u newSize(event.size.width, event.size.height);

            pageManager.UpdateAllLayouts(newSize);

            UpdateGlobalLayout(newSize);
        }

        VolumeChange volChange = volumeControl.HandleEvent(event, window);
        if (volChange.changed) {
            audio.SetVolume(volChange.value);
        }
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (navBar.ContainsToggle(mousePos)) {
                isMenuOpen = !isMenuOpen; 
                navBar.Update(isMenuOpen);
            }
            else if (isMenuOpen) {
                auto target = navBar.HandleClick(mousePos);
                pageManager.ChangeCurrentPage(target);
                currentPage = pageManager.GetCurrentPage();
          
                if (!navBar.GetFullSize().contains(mousePos)) {
                    isMenuOpen = !isMenuOpen;
                    navBar.Update(isMenuOpen);
                }
            }
            else if(!isMenuOpen || !navBar.Contains(mousePos)) {
                if (navButtonNext.Contains(mousePos)) {
                    audio.Next();
                }
                if (navButtonPrev.Contains(mousePos)) {
                    audio.Previous();
                }


                if (playButton.Contains(mousePos)) {
                    audio.TogglePause();
                }

                double seekPct = 0.0;
                if (progressBar.TrySeek(mousePos, seekPct)) {
                    audio.Seek(seekPct);
                }

                currentPage->HandleClick(mousePos);
            }
        }
        if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));

            UpdateGlobalLayout(window.getSize());

            currentPage->UpdateLayout(window.getSize());
        }
    }
}

void Application::Update() {
    audio.UpdateAutoPlay();

    PlaybackInfo info; 
    info.isPlaying = audio.IsPlaying();
    info.hasStopped = audio.HasStopped();
    info.pos = audio.GetPositionSecs();
    info.len = audio.GetLengthSecs();
    pageManager.PageUpdate(info);

    progressBar.Update(info);
    volumeControl.Update(audio.GetVolume());
}

void Application::Render() {
    window.clear(sf::Color(25, 25, 30));

    currentPage->Draw(window);

    window.draw(footerBackground);

    playButton.Draw(window, audio.IsPlaying());
    navButtonNext.Draw(window);
    navButtonPrev.Draw(window);
    progressBar.Draw(window);
    volumeControl.Draw(window);
    navBar.Draw(window);

    window.display();
}