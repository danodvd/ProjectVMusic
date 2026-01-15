#include "Main.hpp"
#include "Application.hpp"
#include "UIUtils.hpp"
#include <iostream>

int main() {
    // 1. Cargar la fuente global antes que nada
    // Asegúrate de que el archivo .ttf esté en la carpeta del ejecutable o ajusta la ruta
    if (!UIUtils::LoadFont("arial.ttf")) {
        std::cerr << "CRITICAL ERROR: Failed to load font." << std::endl;
        return -1;
    }

    // 2. Iniciar la aplicación
    Application app;
    return app.Run();
}