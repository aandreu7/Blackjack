#pragma once

#include "puerto.h"

class Motor;

class Menu
{
public:

    Menu(Motor* motor);
    void draw();
    void handleEvents();
    void run();

    bool getShowMenu() const { return showMenu; }

private:
    
    Motor* motor;

    std::vector<Button*> menuButtons;

	MenuButtonOptions selectedOption;

    sf::Texture* backgroundTexture;
    sf::Sprite* backgroundSprite;

    bool showMenu;
};