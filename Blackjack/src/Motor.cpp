#include "Motor.h"

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

Motor::Motor()
{
	// Loads card textures
	cardMap = loadTexturesFromDirectory(cardFolder, cardTextures, cardSprites);

	desktop = sf::VideoMode::getDesktopMode();

	width = desktop.width * 0.8f;
	height = desktop.height * 0.8f;
}

void Motor::goAhead()
{
	// Creates the window
	window.create(sf::VideoMode(width, height), "Blackjack");

	// Enables V-Sync (syncronizes the refresh rate of the graphics card with the refresh rate of the monitor)
	window.setVerticalSyncEnabled(true);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		jugar(this);

	}
}