#pragma once

#include "puerto.h"

#include "Menu.h"

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
	sf::Texture* victoryBackgroundTexture;
	sf::Texture* defeatBackgroundTexture;
	sf::Event eventSF;
	sf::Font font;
	sf::SoundBuffer card_place_sound;
	sf::SoundBuffer chips_stack_sound;
	sf::Sound sound;

	unsigned int width;
	unsigned int height;

	unsigned int sleepTime;

	std::vector<Button*> gameButtons;

	Menu* mainMenu;

	Partida* partida;

public:

	Motor();

	~Motor();

	void goAhead();

	const ButtonOptions awaitForInput(bool preGame, Oponente* oponente, Crupier* crupier, int apuestaTotal = -1);

	void sleepGame(bool switchBackground=false);

	int getCardKey(int valor, int palo) const { return (valor + palo * 100); }

	const ButtonOptions execGameButtons(const int initOption, const int endOption);

	void windowResized(sf::Sprite* background);

	void initWindow();

	float showCard(int cardKey, int posX, int posY);

	void showText(const std::string& text, int posX, int posY);

	void showPot(int pot);

	void showEndGameBackground(bool victory);
	
	void resetButtons();

	void playSound(char soundIndex);

	void adjustBackground(sf::Sprite* background);

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
	sf::Event& getEventSF() { return eventSF; }
	sf::Font& getFont() { return font; }
	const unsigned int getWidth() const { return width; }
	const unsigned int getHeight() const { return height; }
};