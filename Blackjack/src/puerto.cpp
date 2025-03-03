#include "puerto.h"

void initBackground(sf::Sprite* background, const sf::Window& window)
{
    // Set initial scale for the background sprite
    background->setScale(
        static_cast<float>(window.getSize().x) / background->getTexture()->getSize().x,
        static_cast<float>(window.getSize().y) / background->getTexture()->getSize().y
    );
    background->setPosition(0, 0);
}

std::unordered_map<int, sf::Sprite*> loadTexturesFromDirectory(int width, int height, const std::string& cardFolder, 
	std::vector<sf::Texture*>& cardTextures, std::vector<sf::Sprite*>& cardSprites)
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

					float factorX = width * 0.2;
					float factorY = height * 0.4;

					sprite->setScale(factorX / static_cast<float>(texture->getSize().x), factorY / static_cast<float>(texture->getSize().y));

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