#pragma once
#include "SFML/Graphics.hpp"

#include "NewAudioManager.hpp" 

#include "PageManager.hpp"
#include "NewPagePrincipal.hpp"
#include "NewPageMyMusic.hpp"
#include "NewPageSongQueue.hpp"
#include "NewPageConfig.hpp"

#include "NavBar.hpp"
#include "ProgressBar.hpp"
#include "PlayButton.hpp" 
#include "ButtonNav.hpp"
#include "VolumeControl.hpp"
#include "InfoPanel.hpp"
#include "EqualizerControl.hpp"

class Application {
public:
    Application();
    ~Application();

    int Run();

private:
    void HandleEvents();
    void Update();
    void Render();
    void UpdateGlobalLayout(sf::Vector2u size);
    void UpdateContentLayout();

private:
    sf::View contentView;
    const float MENU_WIDTH = 250.f;

    sf::RenderWindow window;
    AudioManager audio;

    sf::RectangleShape footerBackground;
    NavBar navBar;
    ProgressBar progressBar;
    PlayButton playButton;  
    VolumeControl volumeControl;
    InfoPanel infoPanel;
    NavButton navButtonNext;
    NavButton navButtonPrev;
    EqualizerControl eqControl;

    PageManager pageManager;
    Page* currentPage;
};