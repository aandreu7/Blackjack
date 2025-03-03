#include "Menu.h"

#include "Motor.h"

Menu::Menu(Motor* motor)
{
	this->motor = motor;
    
    int offset = 0;
    for (int i = static_cast<int>(MenuButtonOptions::Jugar); i <= static_cast<int>(MenuButtonOptions::Salir); i++)
    {
        std::string text;

        switch (i)
        {
        case MenuButtonOptions::Jugar:
			text = "Jugar";
			break;
        case MenuButtonOptions::Salir:
			text = "Salir";
            break;
        default:
            text = "ERROR";
            break;
        }
        Button* b = new Button(motor->getWidth() / 2 + offset, motor->getHeight() / 2, BUTTON_SIZE, BUTTON_SIZE, text, "./assets/golden.png");
        menuButtons.push_back(b);
        offset += BUTTON_SIZE;
    }

	selectedOption = MenuButtonOptions::NoneMenuButtonOptions;

    showMenu = true;

    backgroundTexture = new sf::Texture();

    if (!backgroundTexture->loadFromFile("./assets/Screens/menu.jpeg"))
        throw ("Background texture could not be loaded");

    backgroundSprite = new sf::Sprite(*backgroundTexture);
}

void Menu::draw()
{
    motor->getWindow().clear();

	motor->getWindow().draw(*backgroundSprite);

    for (int i = MenuButtonOptions::Jugar; i <= MenuButtonOptions::Salir; i++)
    {
        MenuButtonOptions option = static_cast<MenuButtonOptions>(i);

		menuButtons[option]->draw(motor->getWindow());
        menuButtons[option]->update(motor->getWindow(), motor->getEventSF());

        if (menuButtons[option]->isPressed(motor->getWindow()))
        {
            selectedOption = option;
            break;
        }
    }

    motor->getWindow().display();
}

void Menu::handleEvents()
{
	if (selectedOption != MenuButtonOptions::NoneMenuButtonOptions)
        showMenu = false;

    while (motor->getWindow().pollEvent(motor->getEventSF()))
    {
        if (motor->getEventSF().type == sf::Event::Closed)
            motor->getWindow().close();

        if (motor->getEventSF().type == sf::Event::Resized)
            motor->windowResized(backgroundSprite);
    }
}

void Menu::run()
{
    initBackground(backgroundSprite, motor->getWindow());

    while (showMenu)
    {
        draw();
        handleEvents();
    }
}