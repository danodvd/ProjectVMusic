#pragma once
#include "SFML/Graphics.hpp"

// CAMBIO 1: Usa el nuevo AudioManager
#include "NewAudioManager.hpp" 

// CAMBIO 2: Asegúrate de que estos nombres de archivo sean correctos
#include "PageManager.hpp"
#include "NewPagePrincipal.hpp"
#include "NewPageMyMusic.hpp"
#include "NewPageSongQueue.hpp"
#include "NewPageConfig.hpp"

// Componentes Globales UI (Archivos individuales, NO UI.hpp)
#include "NavBar.hpp"
#include "ProgressBar.hpp"
#include "PlayButton.hpp" // Verifica si el archivo se llama PlayButton.hpp o ButtonPlay.hpp
#include "ButtonNav.hpp"
#include "VolumeControl.hpp"
#include "InfoPanel.hpp" // Descomenta si usas el panel lateral

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

private:

    sf::RenderWindow window;
    AudioManager audio;

    // UI Components
    sf::RectangleShape footerBackground;
    NavBar navBar;
    ProgressBar progressBar;
    PlayButton playButton;   // Asegúrate que la clase se llame PlayButton
    VolumeControl volumeControl;
    InfoPanel infoPanel;
    NavButton navButtonNext;
    NavButton navButtonPrev;

    PageManager pageManager;
    Page* currentPage;
};