#include "Main.hpp"
#include "Application.hpp"
#include "UIUtils.hpp"
#include <iostream>

int main() {
    if (!UIUtils::LoadFont("arial.ttf")) {
        std::cerr << "CRITICAL ERROR: Failed to load font." << std::endl;
        return -1;
    }

    Application app;
    return app.Run();
}