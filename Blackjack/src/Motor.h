#pragma once

#include "puerto.h"

class Oponente;
class Crupier;

class Partida;

class Motor
{
private:

	std::string cardFolder = "./assets/PlayingCards";
	std::vector<sf::Texture*> cardTextures;
	std::vector<sf::Sprite*> cardSprites;
	std::unordered_map<int, sf::Sprite*> cardMap;

	sf::RenderWindow window;
	sf::View view;
	sf::VideoMode desktop;
	sf::Texture* backgroundTexture;
	sf::Sprite* backgroundSprite;

	unsigned int width;
	unsigned int height;

	unsigned int sleepTime;

	std::vector<Button*> pregameButtons;
	std::vector<Button*> gameButtons;

	Partida* partida;

	bool gameStarted;

	void preGrame();

public:

	Motor();

	~Motor() { cardTextures.clear(); cardSprites.clear(); cardMap.clear(); gameButtons.clear(); pregameButtons.clear(); }

	void goAhead();

	ButtonOptions awaitForInput(Oponente* oponente, Crupier* crupier);

	void sleepGame(bool mostrarCartasCrupier);

	int getCardKey(int valor, int palo) const { return (valor + palo * 100); }

	ButtonOptions execGameButtons();

	void windowResized(const sf::Event& eventSF);

	void initWindow();

	float showCard(int cardKey, int posX, int posY);

	void showText(const std::string& text, int posX, int posY);

    // Getters
	const std::string& getCardFolder() const { return cardFolder; }	
	const std::vector<sf::Texture*>& getCardTextures() const { return cardTextures; }
	const std::vector<sf::Sprite*>& getCardSprites() const { return cardSprites; }
	std::unordered_map<int, sf::Sprite*>& getCardMap() { return cardMap; }
	sf::RenderWindow& getWindow() { return window; }
	sf::View& getView() { return view; }
	const sf::VideoMode& getDesktop() const { return desktop; }
	const sf::Texture* getBackgroundTexture() const { return backgroundTexture; }
	const sf::Sprite* getBackgroundSprite() const { return backgroundSprite; }
	const unsigned int getWidth() const { return width; }
	const unsigned int getHeight() const { return height; }
};