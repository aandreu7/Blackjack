#pragma once

#include <SFML/Graphics.hpp>

#include "assert.h"
#include <Windows.h>

#include <filesystem>
namespace fs = std::filesystem;

#include <iostream>
#include <ostream>
#include <locale.h>
#include <ctime>

#include <unordered_map>

#define NUM_PALOS 4
#define NUM_CARTAS 52

#define NUM_BUTTONS 3

namespace simbolosPalo
{
	const char Picas = 'P';
	const char Diamantes = 'D';
	const char Treboles = 'T';
	const char Corazones = 'C';
}

typedef enum
{
	NonePalo = -1,
	Picas,
	Diamantes,
	Treboles,
	Corazones
}Palo;

typedef enum //CUIDADO: NO SON LOS VALORES REALES. PARA EL VALOR REAL HAY QUE SUMAR 1
{
	NoneTipo = -1,
	tAs = 0,
	tX = 1,
	tJ = 2,
	tQ = 3,
	tK = 4
}Tipo;

typedef enum
{
	vJ = 11,
	vQ,
	vK,
	vAs
}ValorTipo;

enum class opcionJugador
{
	PedirCarta,
	Plantarse,
	Doblar
};

enum class rolJugador
{
	None,
	Oponente,
	Crupier
};

typedef enum
{
    None = -1,
    Pedir,
    Doblar,
    Pasar
}ButtonOptions;


template <typename T>
void freeMemory(T*& ptr, bool isArray = false) 
{
    if (ptr != nullptr) 
    {
        if (isArray)
            delete[] ptr;
        else
            delete ptr;
        ptr = nullptr;
    }
}

class Button 
{
private:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
    sf::Font font;
    sf::Color defaultColor;
    sf::Color hoverColor;
    sf::Color clickColor;
    
    bool isHovered;
    bool isClicked;

public:
    Button(float x, float y, float width, float height, const std::string& text) 
    {
        buttonShape.setSize(sf::Vector2f(width, height));
        buttonShape.setPosition(x, y);

        if (!font.loadFromFile("./assets/PlayfairDisplay-Black.ttf"))
            std::cerr << "Source " << std::endl;

        buttonText.setFont(font);
        buttonText.setString(text);
        buttonText.setCharacterSize(24);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition(x + width / 4, y + height / 4);

        defaultColor = sf::Color(100, 100, 250);
        hoverColor = sf::Color(150, 150, 255);
        clickColor = sf::Color(50, 50, 200);

        buttonShape.setFillColor(defaultColor);

        isHovered = false;
        isClicked = false;
    }

    void update(sf::RenderWindow& window)
    {
        isClicked = false;

        if (buttonShape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
        {
            isHovered = true;

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                isClicked = true;
            else if (isClicked)
                isClicked = false;
        }
        else
            isHovered = false;

        if (isClicked)
            buttonShape.setFillColor(clickColor);
        else if (isHovered)
            buttonShape.setFillColor(hoverColor);
        else
            buttonShape.setFillColor(defaultColor);
    }

    void draw(sf::RenderWindow& window) 
    {
        window.draw(buttonShape);
        window.draw(buttonText);
    }

    bool isPressed(sf::RenderWindow& window) 
    {
        return isClicked;
    }

    const std::string& getText() const { return buttonText.getString(); }
};