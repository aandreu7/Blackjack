#pragma once

#include "puerto.h"

#include "Partida.h"

class Motor
{
private:

	std::string cardFolder = "./assets/PlayingCards";
	std::vector<sf::Texture*> cardTextures;
	std::vector<sf::Sprite*> cardSprites;
	std::unordered_map<int, sf::Sprite*> cardMap;


	sf::RenderWindow window;
	sf::VideoMode desktop;

	unsigned int width;
	unsigned int height;

public:

	Motor();

	void goAhead();

	int getCardKey(int valor, int palo) { return (valor + palo * 100); }

    // Getters
	const std::string& getCardFolder() const { return cardFolder; }	
	const std::vector<sf::Texture*>& getCardTextures() const { return cardTextures; }
	const std::vector<sf::Sprite*>& getCardSprites() const { return cardSprites; }
	std::unordered_map<int, sf::Sprite*>& getCardMap() { return cardMap; }
	sf::RenderWindow& getWindow() { return window; }
	const sf::VideoMode& getDesktop() const { return desktop; }
	const unsigned int getWidth() const { return width; }
	const unsigned int getHeight() const { return height; }
};