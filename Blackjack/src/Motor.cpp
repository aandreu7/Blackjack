#include "Motor.h"

#include "Jugador.h"

#include "Partida.h"

Motor::~Motor()
{
	cardTextures.clear(); 
	cardSprites.clear(); 
	cardMap.clear(); 
	gameButtons.clear();
	freeMemory(backgroundTexture); 
	freeMemory(victoryBackgroundTexture); 
	freeMemory(defeatBackgroundTexture);
}

void Motor::showText(const std::string& text, int posX, int posY) 
{
	sf::Text displayText;
	displayText.setFont(font);
	displayText.setString(text);
	displayText.setCharacterSize(50);
	displayText.setFillColor(sf::Color::White);
	displayText.setPosition(posX, posY);
	
	int textSize = static_cast<int>(window.getSize().y * 0.03f);
	displayText.setCharacterSize(textSize);

	window.draw(displayText);
}

void Motor::showEndGameBackground(bool victory)
{
	if (victory)
		backgroundSprite->setTexture(*victoryBackgroundTexture, true);
	else
		backgroundSprite->setTexture(*defeatBackgroundTexture, true);

	adjustBackground(backgroundSprite);

	window.draw(*backgroundSprite);
}

void Motor::adjustBackground(sf::Sprite* background) // Uses both menu and game backgrounds
{
	sf::Vector2u dimWindow = window.getSize();
	sf::Vector2u dimTexture = background->getTexture()->getSize();

	if (dimTexture.x == 0 || dimTexture.y == 0) return;

	float escalaX = static_cast<float>(dimWindow.x) / dimTexture.x;
	float escalaY = static_cast<float>(dimWindow.y) / dimTexture.y;

	background->setScale(escalaX, escalaY);
}

void Motor::windowResized(sf::Sprite* background) // Uses both menu and game backgrounds
{
	// Adjusts the view to the new window size
	view.setSize(eventSF.size.width, eventSF.size.height);
	view.setCenter(eventSF.size.width / 2.f, eventSF.size.height / 2.f);
	window.setView(view);

	// Adjusts the background to the new window size
	adjustBackground(background);
}

void Motor::sleepGame(bool switchBackground)
{
	sf::Clock clock;

	// Shows objects
	window.display(); // Shows the background and the cards previously drawn

	while (clock.getElapsedTime().asSeconds() < sleepTime && window.isOpen())
	{
		while (window.pollEvent(eventSF))
		{
			if (eventSF.type == sf::Event::Closed)
				window.close();

			if (eventSF.type == sf::Event::Resized)
				windowResized(backgroundSprite);
		}
	}

	// Clears the window
	window.clear();

	// If end game (when victory/defeat background is set), then switch back to the default background
	if (switchBackground)
	{
		backgroundSprite->setTexture(*backgroundTexture, true);

		adjustBackground(backgroundSprite);

		window.draw(*backgroundSprite);
	}
	
	// Prints the background (prepares it for the next sleepGame call)
	window.draw(*backgroundSprite);
}

Motor::Motor()
{
	// Initializes random seed
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	desktop = sf::VideoMode::getDesktopMode();

	width = desktop.width * 0.8f;
	height = desktop.height * 0.8f;

	view = sf::View(sf::FloatRect(0.f, 0.f, width, height));

	// Loads card textures
	cardMap = loadTexturesFromDirectory(width, height, cardFolder, cardTextures, cardSprites);
	
	// Loads backgrounds texture
	backgroundTexture = new sf::Texture();

	if (!backgroundTexture->loadFromFile("./assets/fondo.png"))
		throw ("Background texture could not be loaded");

	backgroundSprite = new sf::Sprite(*backgroundTexture);

	victoryBackgroundTexture = new sf::Texture();

	if (!victoryBackgroundTexture->loadFromFile("./assets/Screens/victory.jpeg"))
		throw ("Background texture could not be loaded");

	defeatBackgroundTexture = new sf::Texture();

	if (!defeatBackgroundTexture->loadFromFile("./assets/Screens/defeat.jpeg"))
		throw ("Background texture could not be loaded");

	// Loads audio
	if (!card_place_sound.loadFromFile("./assets/Audio/card-place-1.ogg"))
		throw ("Card place sound could not be loaded");
	if (!chips_stack_sound.loadFromFile("./assets/Audio/chips-stack-3.ogg"))
		throw("Chip stack sound could not be loaded");
	
	// Creates buttons
	gameButtons.resize(NUM_BUTTONS, nullptr);

	// PREGAME BUTTONS
	Button* subeB = new Button(0.25 * width, height / 2, 0.15 * width, 0.18 * height, std::string("Subir"), "./assets/pokerchips/pokerchip1.png");
	Button* bajaB = new Button(0.5 * width, height / 2, 0.15 * width, 0.18 * height, std::string("Bajar"), "./assets/pokerchips/pokerchip2.png");
	Button* empiezaB = new Button(0.75 * width, height / 2, 0.15 * width, 0.18 * height, std::string("Empezar"), "./assets/pokerchips/pokerchip3.png");

	// GAME BUTTONS
	Button* pedirB = new Button(0.25 * width, height / 2, 0.15 * width, 0.18 * height, std::string("Pedir"), "./assets/pokerchips/pokerchip1.png");
	Button* doblarB = new Button(0.5 * width, height / 2, 0.15 * width, 0.18 * height, std::string("Doblar"), "./assets/pokerchips/pokerchip2.png");
	Button* pasarB = new Button(0.75 * width, height / 2, 0.15 * width, 0.18 * height, std::string("Pasar"), "./assets/pokerchips/pokerchip3.png");

	gameButtons[ButtonOptions::Pedir] = pedirB;
	gameButtons[ButtonOptions::Doblar] = doblarB;
	gameButtons[ButtonOptions::Pasar] = pasarB;
	gameButtons[ButtonOptions::Subir] = subeB;
	gameButtons[ButtonOptions::Bajar] = bajaB;
	gameButtons[ButtonOptions::Empezar] = empiezaB;

	sleepTime = 3; // Sleeps 3 seconds every time a new card is shown

	mainMenu = new Menu(this);

	partida = new Partida(this);

	if (!font.loadFromFile("./assets/PlayfairDisplay-Black.ttf"))
		std::cerr << "Source " << std::endl;
}

void Motor::playSound(char soundIndex)
{
	// soundIndex == 0 --> Card placed
	// soundIndex == 1 --> Chip stack
	switch (soundIndex)
	{
	case 0:
		sound.setBuffer(card_place_sound);
		break;
	case 1:
		sound.setBuffer(chips_stack_sound);
		break;
	default:
		break;
	}
	
	sound.play();
}

const ButtonOptions Motor::execGameButtons(const int initOption, const int endOption)
{
	ButtonOptions selectedOption = ButtonOptions::NoneButtonOptions;

	for (int i = initOption; i <= endOption; i++)
	{
		if (!partida->doblarPermitido && i == static_cast<int>(ButtonOptions::Doblar))
			continue;

		ButtonOptions option = static_cast<ButtonOptions>(i);

		gameButtons[option]->draw(window);
		gameButtons[option]->update(window, eventSF);

		if (gameButtons[option]->isPressed(window))
			selectedOption = option;
	}

	return selectedOption;
}

float Motor::showCard(int cardKey, int posX, int posY)
{
	sf::Sprite* sprite = cardMap[cardKey];

	sprite->setPosition(posX, posY);

	window.draw(*sprite);

	float witdh = sprite->getGlobalBounds().width;

	return witdh;
}

void Motor::showPot(int pot)
{
	showText("BOTE ACTUAL: " + std::to_string(pot), width * 0.9, height * 0.3);
}

const ButtonOptions Motor::awaitForInput(bool preGame, Oponente* oponente, Crupier* crupier, int apuestaTotal)
{
	if ((preGame && apuestaTotal == -1) || (!preGame && apuestaTotal != -1 ))
		throw ("Non expected parameters values");

	ButtonOptions opcion = ButtonOptions::NoneButtonOptions;

	while (opcion == ButtonOptions::NoneButtonOptions && window.isOpen())
	{
		while (window.pollEvent(eventSF))
		{
			if (eventSF.type == sf::Event::Closed)
				window.close();

			if (eventSF.type == sf::Event::Resized)
				windowResized(backgroundSprite);
		}

		// Prints the background
		window.draw(*backgroundSprite);

		if (preGame) // BETTING PHASE
		{
			oponente->mostrar(this);
			crupier->mostrar(this, false);
			showText("APUESTA TOTAL: " + std::to_string(apuestaTotal), 50.0f, 20.0f * 1.25);
			opcion = execGameButtons(ButtonOptions::Subir, ButtonOptions::Empezar); // Updates button states and returns the selected option
		}
			
		else // GAME PHASE
		{
			if (partida != nullptr)
				partida->mostrarEstadoJuego(false, false);

			opcion = execGameButtons(ButtonOptions::Pedir, ButtonOptions::Pasar); // Updates button states and returns the selected option
		}

		// Shows objects
		window.display();

		// Clears the window
		window.clear();
	}

	// Prints the background (prepares it for the next sleepGame call)
	window.draw(*backgroundSprite);

	return opcion;
}

void Motor::resetButtons()
{
	for (auto it : gameButtons)
		it->reset();
}

void Motor::initWindow()
{
	// Creates the window
	window.create(sf::VideoMode(width, height), "Blackjack");

	sf::Image icon;
	if (!icon.loadFromFile("./assets/icon.png")) 
	{
		std::cerr << "Error: icon could not be loaded." << std::endl;
		return;
	}

	// Configure window icon
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	// Enables V-Sync (synchronizes the refresh rate of the graphics card with the refresh rate of the monitor)
	window.setVerticalSyncEnabled(true);
}

void Motor::goAhead()
{
	initWindow();

	mainMenu->run();

	initBackground(backgroundSprite, window);

	while (window.isOpen())
	{
		// Call game logic
		partida->jugar();

		resetButtons();
	}
}
