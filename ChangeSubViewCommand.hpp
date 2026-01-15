#pragma once
#include "Command.hpp"
#include "NewPageMyMusic.hpp"

class ChangeSubViewCommand : public Command {
public:
    // El comando necesita saber QUÉ página modificar y a QUÉ vista cambiar
    ChangeSubViewCommand(PageMyMusic& page, PageMyMusic::SubView targetView)
        : page(page), targetView(targetView) {
    }

    void Execute() override {
        // Le ordenamos a la página que cambie su vista
        page.SetCurrentView(targetView);
    }

private:
    PageMyMusic& page;
    PageMyMusic::SubView targetView;
};