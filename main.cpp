#include "Baraja.h"
#include "Jugador.h"

int main()
{
	std::srand(std::time(nullptr));
	Baraja b;
	b.barajar();

	Jugador Oponente(b.repartirCarta(), b.repartirCarta());
	Jugador Crupier(b.repartirCarta(), b.repartirCarta());
	std::cout << "CARTAS JUGADOR: " << std::endl;
	std::cout << Oponente;
	std::cout << "CARTA CRUPIER: " << std::endl;
	std::cout << Crupier;
	char option = ' ';
	std::cout << "TU PUNTUACION ES: " << Oponente.getPuntuacion() << ". PEDIR CARTA ? (S / N)" << std::endl;
	std::cin >> option;
	if (option == 'S')
	{
		Oponente.anadirCarta(b.repartirCarta());
		std::cout << "CARTAS JUGADOR: " << std::endl;
		std::cout << Oponente;
	}
	

	



	


	return 0;
}