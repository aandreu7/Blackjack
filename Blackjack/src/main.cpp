#include "puerto.h"

#include "Baraja.h"
#include "Carta.h"
#include "Jugador.h"

void jugadorGana(Jugador* g, Jugador* p, bool empate)
{
	if (!empate)
	{
		if (g->getRol() == rolJugador::Oponente)
			std::cout << "OPONENTE GANA" << std::endl;
		else if (g->getRol() == rolJugador::Crupier)
			std::cout << "CRUPIER GANA" << std::endl;
		else
			throw std::invalid_argument("Ganador no valido");

		g->setBanca(g->getBanca() + g->getApuesta() + p->getApuesta());
	}
	else
	{
		std::cout << "EMPATE" << std::endl;
		g->setBanca(g->getApuesta());
		p->setBanca(p->getApuesta());
	}

	g->setApuesta(0);
	p->setApuesta(0);
}

void repartirCartaAMano(Baraja* b, Jugador* j)
{
	j->anadirCarta(b->repartirCarta());
}

void mostrarEstadoJuego(Jugador* Oponente, Jugador* Crupier, bool mostrarCartasCrupier=false)
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
	std::cout << "CARTAS JUGADOR: " << std::endl;
	std::cout << *Oponente;
	std::cout << "CARTA CRUPIER: " << std::endl;
	if (mostrarCartasCrupier)
		std::cout << *Crupier;
	else
	{
		std::cout << *(Crupier->getCartas());
		std::cout << "??" << std::endl;
	}
	Sleep(3000);
}

void mostrarOpciones(bool doblarPermitido)
{
	std::cout << std::endl;
	std::cout << "OPCIONES: " << std::endl;
	std::cout << "1. PEDIR CARTA (X)" << std::endl;
	std::cout << "2. PLANTARSE (C)" << std::endl;
	if (doblarPermitido)
		std::cout << "3. DOBLAR (V)" << std::endl;
}

opcionJugador leerJugada(bool doblarPermitido)
{
	/*
	LISTA OPCIONES:
	1. PEDIR CARTA (X)
	2. PLANTARSE (C)
	3. DOBLAR (V)
	*/

	char opcion = -1;
	std::cin >> opcion;

	switch (opcion)
	{
	case 'X':
		return opcionJugador::PedirCarta;
	case 'C':
		return opcionJugador::Plantarse;
	case 'V':
		if (doblarPermitido)
			return opcionJugador::Doblar;
	default:
		throw std::invalid_argument("Opcion no valida");
		break;
	}
}

void crupierJuega(Jugador* Crupier, Jugador* Oponente, Baraja* b)
{
	assert(Crupier != nullptr && b != nullptr && Crupier->getRol() == rolJugador::Crupier);

	mostrarEstadoJuego(Oponente, Crupier, true); // MOSTRAR LA CARTA OCULTA

	while (Crupier->getPuntuacion() < 17) // REPARTIR NUEVAS CARTAS
	{
		repartirCartaAMano(b, Crupier);
		mostrarEstadoJuego(Oponente, Crupier, true);
	}
}

void oponenteJuega(Jugador* Oponente, Jugador* Crupier, Baraja* b)
{
	assert(Oponente != nullptr && b != nullptr && Oponente->getRol() == rolJugador::Oponente);

	bool stop = false;

	do
	{
		repartirCartaAMano(b, Oponente);
		mostrarEstadoJuego(Oponente, Crupier);

		if (Oponente->getPuntuacion() >= 21)
			stop = true;
		else
		{
			mostrarOpciones(false);
			opcionJugador op = leerJugada(false);
			if (op == opcionJugador::Plantarse)
				stop = true;
		}
	} while (!stop);
}

void determinaGanador(Jugador* Crupier, Jugador* Oponente)
{
	mostrarEstadoJuego(Oponente, Crupier, true);

	if (Oponente->getPuntuacion() > 21) // OPONENTE SE PASA -> CRUPIER GANA
		jugadorGana(Crupier, Oponente, false);

	else if (Crupier->getPuntuacion() > 21) // CRUPIER SE PASA -> OPONENTE GANA
		jugadorGana(Oponente, Crupier, false);

	else if (Oponente->getPuntuacion() > Crupier->getPuntuacion()) // OPONENTE GANA
		jugadorGana(Oponente, Crupier, false);

	else if (Oponente->getPuntuacion() < Crupier->getPuntuacion()) // CRUPIER GANA
		jugadorGana(Crupier, Oponente, false);

	else // EMPATE
		jugadorGana(Crupier, Oponente, true);
}

bool iniciarJuego(Baraja* b, Jugador* Oponente, Jugador* Crupier, opcionJugador& op)
{
	/*
	DEVUELVE:
	true: JUEGO CONTINUA
	false: JUEGO TERMINADO
	*/

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	b->barajar();

	repartirCartaAMano(b, Oponente);
	repartirCartaAMano(b, Oponente);

	repartirCartaAMano(b, Crupier);
	repartirCartaAMano(b, Crupier);

	mostrarEstadoJuego(Oponente, Crupier);

	if (Oponente->comprovarBlackjack())
	{
		mostrarEstadoJuego(Oponente, Crupier, true);
		if (Crupier->comprovarBlackjack()) // LOS DOS TIENEN BLACKJACK
			jugadorGana(Oponente, Crupier, true);
		else
			jugadorGana(Oponente, Crupier, false);
		return false;
	}
	else if (Crupier->comprovarBlackjack())
	{
		mostrarEstadoJuego(Oponente, Crupier, true);
		jugadorGana(Crupier, Oponente, false);
		return false;
	}

	mostrarOpciones(true);

	op = leerJugada(true);

	return true;
}

void jugar()
{
	Baraja b;
	Jugador Oponente(rolJugador::Oponente), Crupier(rolJugador::Crupier);
	opcionJugador op;

	if (iniciarJuego(&b, &Oponente, &Crupier, op))
	{
		if (op == opcionJugador::Doblar)
		{
			Oponente.setApuesta(Oponente.getApuesta() * 2);
			Crupier.setApuesta(Crupier.getApuesta() * 2);
			repartirCartaAMano(&b, &Oponente);
			mostrarEstadoJuego(&Oponente, &Crupier);
		}
		else if (op == opcionJugador::PedirCarta)
			oponenteJuega(&Oponente, &Crupier, &b);
			

		if (Oponente.getPuntuacion() < 21)
			crupierJuega(&Crupier, &Oponente, &b);

		determinaGanador(&Crupier, &Oponente);
	}
}


int main()
{
	while (true)
	{
		jugar();
		Sleep(5000);
	}

	/*
	std::vector<sf::Texture> cardTextures;
	std::vector<sf::Sprite> cardSprites;

	std::string cardFolder = "C:/Users/andre/Desktop/G/Blackjack/Blackjack/assets/Playing Cards/PNG-cards-1.3";
	

	try
	{
		for (const auto& entry : fs::directory_iterator(cardFolder))
		{
			if (entry.path().extension() == ".png")
			{
				sf::Texture texture;
				if (texture.loadFromFile(entry.path().string()))
				{
					std::cout << "Image loaded: " << entry.path().filename().string() << std::endl;
					cardTextures.push_back(texture);

					sf::Sprite sprite(texture);
					sprite.setScale(2.0f, 2.0f);
					cardSprites.push_back(sprite);
				}
				else
					std::cerr << "Error loading image: " << entry.path().filename().string() << std::endl;
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Directory access error: " << e.what() << std::endl;
	}
		
	float x = 10.f;
	float y = 10.f;



	sf::Texture texture;  // Creamos un objeto sf::Texture
	if (!texture.loadFromFile("C:\\Users\\andre\\Desktop\\G\\Blackjack\\Blackjack\\assets\\2_of_clubs.png")) {
		// Si no se puede cargar la textura, muestra un error
		std::cerr << "Error loading texture!" << std::endl;
	}
	sf::Sprite sprite(texture);  // Crear un sprite con la textura cargada
	sprite.setPosition(100.f, 150.f);  // Coloca el sprite en la posición (100, 150)




	// Creates the window

	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

	unsigned int width = desktop.width * 0.8f;
	unsigned int height = desktop.height * 0.8f;

	sf::RenderWindow window(sf::VideoMode(width, height), "Blackjack");
	
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

		window.clear();  // Limpiar la ventana
		window.draw(sprite);
		window.display();
	}
	*/

	return 0;
}
