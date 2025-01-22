#include "Motor.h"

#include "Jugador.h"

#include "Partida.h"

std::unordered_map<int, sf::Sprite*> loadTexturesFromDirectory(const std::string& cardFolder, std::vector<sf::Texture*>& cardTextures, std::vector<sf::Sprite*>& cardSprites)
{
	std::unordered_map<int, sf::Sprite*> cardMap;

	try
	{
		for (auto& entry : fs::directory_iterator(cardFolder))
		{
			if (entry.is_regular_file() && entry.path().extension() == ".png")
			{
				std::cout << "Loading: " << entry.path().string() << std::endl;

				sf::Texture* texture = new sf::Texture;
				if ((*texture).loadFromFile(entry.path().string()))
				{
					std::cout << "Loaded: " << entry.path().filename().string() << std::endl;
					cardTextures.push_back(texture);

					sf::Sprite* sprite = new sf::Sprite(*(cardTextures.back()));
					cardSprites.push_back(sprite);

					std::string filename = entry.path().stem().string();

					if (filename == "Reverse") // Special card
					{
						cardMap[0] = sprite;
						continue;
					}

					char paloChar = filename.back();
					int valor = std::stoi(filename.substr(0, filename.size() - 1));

					int paloValue;

					switch (paloChar)
					{
					case simbolosPalo::Corazones:
						paloValue = Palo::Corazones;
						break;
					case simbolosPalo::Diamantes:
						paloValue = Palo::Diamantes;
						break;
					case simbolosPalo::Picas:
						paloValue = Palo::Picas;
						break;
					case simbolosPalo::Treboles:
						paloValue = Palo::Treboles;
						break;
					default:
						throw ("Invalid card suit");
					}

					int cardId = paloValue * 100 + valor;

					cardMap[cardId] = sprite;
				}
				else
				{
					std::cerr << "Failed to load: " << entry.path().filename().string() << std::endl;
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error while accessing directory: " << e.what() << std::endl;
	}

	return cardMap;
}

void Motor::showText(const std::string& text, int posX, int posY) 
{
	static sf::Font font;

	if (!font.loadFromFile("./assets/PlayfairDisplay-Black.ttf"))
		std::cerr << "Source " << std::endl;

	sf::Text displayText;
	displayText.setFont(font);
	displayText.setString(text);
	displayText.setCharacterSize(50);
	displayText.setFillColor(sf::Color::White);
	displayText.setPosition(posX, posY);

	window.draw(displayText);
}

void Motor::windowResized(const sf::Event& eventSF)
{
	// Adjusts the view to the new window size
	view.setSize(eventSF.size.width, eventSF.size.height);
	view.setCenter(eventSF.size.width / 2.f, eventSF.size.height / 2.f);
	window.setView(view);

	// Adjusts the background to the new window size
	backgroundSprite->setScale(
		static_cast<float>(eventSF.size.width) / backgroundTexture->getSize().x,
		static_cast<float>(eventSF.size.height) / backgroundTexture->getSize().y
	);
	backgroundSprite->setPosition(0, 0);
}

void Motor::sleepGame(bool mostrarCartasCrupier)
{
	sf::Clock clock;

	// Shows objects
	window.display(); // Shows the background and the cards previously drawn

	while (clock.getElapsedTime().asSeconds() < sleepTime && window.isOpen())
	{
		sf::Event eventSleep;
		while (window.pollEvent(eventSleep))
		{
			if (eventSleep.type == sf::Event::Closed)
				window.close();

			if (eventSleep.type == sf::Event::Resized)
				windowResized(eventSleep);
		}
	}

	// Clears the window
	window.clear();

	// Prints the background (prepares it for the next sleepGame call)
	window.draw(*backgroundSprite);
}

Motor::Motor()
{
	// Loads card textures
	cardMap = loadTexturesFromDirectory(cardFolder, cardTextures, cardSprites);

	desktop = sf::VideoMode::getDesktopMode();

	width = desktop.width * 0.8f;
	height = desktop.height * 0.8f;

	view = sf::View(sf::FloatRect(0.f, 0.f, width, height));

	backgroundTexture = new sf::Texture();

	if (!backgroundTexture->loadFromFile("./assets/fondo.png"))
		throw ("Background texture could not be loaded");

	backgroundSprite = new sf::Sprite(*backgroundTexture);

	// PREGAME BUTTONS
	Button* apostarB = new Button(width / 2 - 100, height / 2, 200, 200, std::string("Apostar"));
	Button* seleccB = new Button(width / 2 - 100, height / 2 + 250, 200, 200, std::string("Seleccionar apuesta"));
	Button* empezarB = new Button(width / 2 - 100, height / 2 + 500, 200, 200, std::string("Empezar"));

	pregameButtons.push_back(apostarB);
	pregameButtons.push_back(seleccB);
	pregameButtons.push_back(empezarB);

	// GAME BUTTONS
	Button* pedirB = new Button(3 * width / 4, height / 2, 200, 200, std::string("Pedir"));
	Button* doblarB = new Button(3 * width / 4 + 250, height / 2, 200, 200, std::string("Doblar"));
	Button* pasarB = new Button(3 * width / 4 + 500, height / 2, 200, 200, std::string("Pasar"));

	gameButtons.push_back(pedirB);
	gameButtons.push_back(doblarB);
	gameButtons.push_back(pasarB);

	sleepTime = 3; // Sleeps 3 seconds every time a new card is shown

	partida = new Partida(this);

	gameStarted = false;
}

ButtonOptions Motor::execGameButtons()
{
	for (int i = static_cast<int>(ButtonOptions::Pedir); i <= static_cast<int>(ButtonOptions::Pasar); i++)
	{
		if (!partida->doblarPermitido && i == static_cast<int>(ButtonOptions::Doblar))
			continue;

		ButtonOptions option = static_cast<ButtonOptions>(i);

		gameButtons[option]->draw(window);
		gameButtons[option]->update(window);

		if (gameButtons[option]->isPressed(window))
			return option;
	}

	return ButtonOptions::None;
}

float Motor::showCard(int cardKey, int posX, int posY)
{
	sf::Sprite* sprite = cardMap[cardKey];

	sprite->setPosition(posX, posY);

	window.draw(*sprite);

	float witdh = sprite->getGlobalBounds().width;

	return witdh;
}

ButtonOptions Motor::awaitForInput(Oponente* oponente, Crupier* crupier)
{
	ButtonOptions opcion = ButtonOptions::None;

	while (opcion == ButtonOptions::None && window.isOpen())
	{
		sf::Event eventSF;
		while (window.pollEvent(eventSF))
		{
			if (eventSF.type == sf::Event::Closed)
				window.close();

			if (eventSF.type == sf::Event::Resized)
				windowResized(eventSF);
		}

		// Prints the background
		window.draw(*backgroundSprite);

		oponente->mostrar(this);
		crupier->mostrar(this, false);

		opcion = execGameButtons(); // Updates button states and returns the selected option

		// Shows objects
		window.display();

		// Clears the window
		window.clear();
	}

	// Prints the background (prepares it for the next sleepGame call)
	window.draw(*backgroundSprite);

	return opcion;
}

void Motor::initWindow()
{
	// Creates the window
	window.create(sf::VideoMode(width, height), "Blackjack");

	// Enables V-Sync (synchronizes the refresh rate of the graphics card with the refresh rate of the monitor)
	window.setVerticalSyncEnabled(true);

	// Set initial scale for the background sprite
	backgroundSprite->setScale(
		static_cast<float>(window.getSize().x) / backgroundTexture->getSize().x,
		static_cast<float>(window.getSize().y) / backgroundTexture->getSize().y
	);
	backgroundSprite->setPosition(0, 0);
}

void Motor::preGrame()
{
	for (Button* button : pregameButtons)
	{
		button->draw(window);
		button->update(window);
	}
	
	// Clears window
	window.clear();
}

void Motor::goAhead()
{
	initWindow();

	while (window.isOpen())
	{
		sf::Event eventSF;
		while (window.pollEvent(eventSF))
		{
			if (eventSF.type == sf::Event::Closed)
				window.close();

			if (eventSF.type == sf::Event::Resized)
				windowResized(eventSF);
		}

		// Prints the background
		window.draw(*backgroundSprite);

		// Call game logic
		partida->jugar();
	}
}
