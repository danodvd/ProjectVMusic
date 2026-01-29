#include "Application.hpp"
#include "LibraryPersistence.hpp"
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
    , eqControl({ 1280.f - 150.f, 720.f - 40.f })
{
    window.setFramerateLimit(60);
    if (!audio.Init()) {
        std::cerr << "Error inicializando Audio Manager" << std::endl;
    }

    std::vector<SongData> savedSongs = LibraryPersistence::LoadLibraryData("library.dat");
    if (!savedSongs.empty()) {
        std::cout << "Loading songs.." << savedSongs.size() << " saved songs.." << std::endl;
        for (const auto& song : savedSongs) {
            audio.AddSong(song, false);
        }
    }

    currentPage = pageManager.GetCurrentPage();

    footerBackground.setFillColor(sf::Color(20, 20, 20, 230));

    UpdateGlobalLayout(window.getSize());
    UpdateContentLayout();
}

Application::~Application() {
    LibraryPersistence::SaveLibrary(audio.GetQueue(), "library.dat");
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

void Application::UpdateContentLayout() {
    sf::Vector2u windowSize = window.getSize();
    float w = static_cast<float>(windowSize.x);
    float h = static_cast<float>(windowSize.y);
    float footerH = 80.f; 


    float startX = isMenuOpen ? MENU_WIDTH : 0.f; 
    float availableWidth = w - startX;
    float availableHeight = h - footerH; 

    contentView.setSize(availableWidth, availableHeight);

    contentView.setCenter(availableWidth / 2.f, availableHeight / 2.f);

    float viewX = startX / w;
    float viewY = 0.f;
    float viewW = availableWidth / w;
    float viewH = availableHeight / h;

    contentView.setViewport(sf::FloatRect(viewX, viewY, viewW, viewH));
    pageManager.UpdateAllLayouts(sf::Vector2u(static_cast<unsigned int>(availableWidth), static_cast<unsigned int>(availableHeight)));
}

void Application::UpdateGlobalLayout(sf::Vector2u size) {
    float w = (float)size.x;
    float h = (float)size.y;

    float footerHeight = 80.f;      
    float footerY = h - footerHeight;

    footerBackground.setSize({ w, footerHeight });
    footerBackground.setPosition(0, footerY);

    progressBar.SetPosition({ 0.f, footerY - 10.f });
    progressBar.SetSize(sf::Vector2f(w, 10));

    float centerY = footerY + (footerHeight / 2.f);

    playButton.SetPosition({ w / 2.f, centerY });

    navButtonPrev.SetPosition({ (w / 2.f) - 150.f, centerY }, NavButton::Direction::Previous);
    navButtonNext.SetPosition({ (w / 2.f) + 150.f, centerY }, NavButton::Direction::Next);

    volumeControl.SetPosition({ w - 250.f, centerY });

    eqControl.SetPosition({ w - 160.f, centerY });

    navBar.SetHeight(footerY - 10);
}

void Application::HandleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (isMenuOpen) {
            navBar.HandleEvent(mousePos, event);
        }

        if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, (float)event.size.width, (float)event.size.height);
            window.setView(sf::View(visibleArea));

            sf::Vector2u newSize(event.size.width, event.size.height);

            UpdateGlobalLayout(newSize);
            UpdateContentLayout(); 
        }

        if (eqControl.HandleEvent(event, window, audio)) {
            continue;
        }

        VolumeChange volChange = volumeControl.HandleEvent(event, window);
        if (volChange.changed) {
            audio.SetVolume(volChange.value);
        }

        sf::Vector2f currentSize((float)window.getSize().x, (float)window.getSize().y);
        sf::View uiView(sf::FloatRect(0.f, 0.f, currentSize.x, currentSize.y));

        sf::Vector2f mouseGlobal = window.mapPixelToCoords(sf::Mouse::getPosition(window), uiView);
        sf::Vector2f mousePage = window.mapPixelToCoords(sf::Mouse::getPosition(window), contentView);

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

            if (navBar.ContainsToggle(mouseGlobal)) {
                isMenuOpen = !isMenuOpen;
                navBar.Update(isMenuOpen);
                UpdateContentLayout();
            }
            else if (isMenuOpen) {
                auto target = navBar.HandleClick(mouseGlobal);
                pageManager.ChangeCurrentPage(target);
                currentPage = pageManager.GetCurrentPage();
                if (!navBar.GetFullSize().contains(mouseGlobal)) {
                    isMenuOpen = !isMenuOpen;
                    navBar.Update(isMenuOpen);
                    UpdateContentLayout();
                }
            }
            else if (!isMenuOpen || !navBar.Contains(mouseGlobal)) {
                if (navButtonNext.Contains(mouseGlobal)) audio.Next();
                if (navButtonPrev.Contains(mouseGlobal)) audio.Previous();
                if (playButton.Contains(mouseGlobal)) audio.TogglePause();

                double seekPct = 0.0;
                if (progressBar.TrySeek(mouseGlobal, seekPct)) {
                    audio.Seek(seekPct);
                }
                currentPage->HandleClick(mousePage);
            }
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

    window.setView(contentView);
    currentPage->Draw(window);

    sf::View uiView(sf::FloatRect(0.f, 0.f, (float)window.getSize().x, (float)window.getSize().y));
    window.setView(uiView);

    window.draw(footerBackground);

    playButton.Draw(window, audio.IsPlaying());
    navButtonNext.Draw(window);
    navButtonPrev.Draw(window);
    volumeControl.Draw(window);
    eqControl.Draw(window);
    navBar.Draw(window);
    progressBar.Draw(window);

    window.display();
}