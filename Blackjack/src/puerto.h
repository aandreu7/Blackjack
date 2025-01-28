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

#include <fstream>
#include <nlohmann/json.hpp>

#include <unordered_map>

#define NUM_PALOS 4
#define NUM_CARTAS 52

#define NUM_BUTTONS 7

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
	Doblar,
    Subir,
    Bajar,
    Apostar,
    Empezar
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
    Pasar,
    Subir,
    Bajar,
    Apostar,
    Empezar
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
    bool wasPressed;

public:
    Button(float x, float y, float width, float height, const std::string& text) 
    {
        buttonShape.setSize(sf::Vector2f(width, height));
        buttonShape.setPosition(x, y);

        if (!font.loadFromFile("./assets/PlayfairDisplay-Black.ttf"))
            std::cerr << "Source " << std::endl;

        buttonText.setFont(font);
        buttonText.setString(text);
        buttonText.setCharacterSize(30);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition(x + width / 4, y + height / 4);

        defaultColor = sf::Color(100, 100, 250);
        hoverColor = sf::Color(150, 150, 255);
        clickColor = sf::Color(50, 50, 200);

        buttonShape.setFillColor(defaultColor);

        isHovered = false;
        isClicked = false;
        wasPressed = false;
    }

    void update(sf::RenderWindow& window, sf::Event& event)
    {
        isClicked = false;

        // Verifica si el cursor está sobre el botón
        if (buttonShape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
        {
            isHovered = true;

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                // Asegura que el click solo se registra una vez
                if (!wasPressed)
                {
                    isClicked = true;
                    wasPressed = true;
                }
            }
        }
        else
            isHovered = false;
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            wasPressed = false;

        // Actualiza el color del botón según el estado
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