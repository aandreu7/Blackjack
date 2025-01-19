#include "Partida.h"

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

void mostrarEstadoJuego(Jugador* Oponente, Jugador* Crupier, Motor* motor, bool mostrarCartasCrupier = false)
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
	std::cout << "CARTAS JUGADOR: " << std::endl;
	std::cout << *Oponente;

	if (motor != nullptr)
	{
		// Cleans the window
		motor->getWindow().clear();
	
		Oponente->mostrar(motor);
		Crupier->mostrar(motor);

		// Shows objects
		motor->getWindow().display();
	}
		

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

void crupierJuega(Jugador* Crupier, Jugador* Oponente, Baraja* b, Motor* motor)
{
	assert(Crupier != nullptr && b != nullptr && Crupier->getRol() == rolJugador::Crupier);

	mostrarEstadoJuego(Oponente, Crupier, motor, true); // MOSTRAR LA CARTA OCULTA

	while (Crupier->getPuntuacion() < 17) // REPARTIR NUEVAS CARTAS
	{
		repartirCartaAMano(b, Crupier);
		mostrarEstadoJuego(Oponente, Crupier, motor, true);
	}
}

void oponenteJuega(Jugador* Oponente, Jugador* Crupier, Baraja* b, Motor* motor)
{
	assert(Oponente != nullptr && b != nullptr && Oponente->getRol() == rolJugador::Oponente);

	bool stop = false;

	do
	{
		repartirCartaAMano(b, Oponente);
		mostrarEstadoJuego(Oponente, Crupier, motor);

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

void determinaGanador(Jugador* Crupier, Jugador* Oponente, Motor* motor)
{
	mostrarEstadoJuego(Oponente, Crupier, motor, true);

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

bool iniciarJuego(Baraja* b, Jugador* Oponente, Jugador* Crupier, opcionJugador& op, Motor* motor)
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

	mostrarEstadoJuego(Oponente, Crupier, motor);

	if (Oponente->comprovarBlackjack())
	{
		mostrarEstadoJuego(Oponente, Crupier, motor, true);
		if (Crupier->comprovarBlackjack()) // LOS DOS TIENEN BLACKJACK
			jugadorGana(Oponente, Crupier, true);
		else
			jugadorGana(Oponente, Crupier, false);
		return false;
	}
	else if (Crupier->comprovarBlackjack())
	{
		mostrarEstadoJuego(Oponente, Crupier, motor, true);
		jugadorGana(Crupier, Oponente, false);
		return false;
	}

	mostrarOpciones(true);

	op = leerJugada(true);

	return true;
}

void jugar(Motor* motor)
{
	Baraja b;
	Jugador Oponente(rolJugador::Oponente), Crupier(rolJugador::Crupier);
	opcionJugador op;

	if (iniciarJuego(&b, &Oponente, &Crupier, op, motor))
	{
		if (op == opcionJugador::Doblar)
		{
			Oponente.setApuesta(Oponente.getApuesta() * 2);
			Crupier.setApuesta(Crupier.getApuesta() * 2);
			repartirCartaAMano(&b, &Oponente);
			mostrarEstadoJuego(&Oponente, &Crupier, motor);
		}
		else if (op == opcionJugador::PedirCarta)
			oponenteJuega(&Oponente, &Crupier, &b, motor);

		if (Oponente.getPuntuacion() < 21)
			crupierJuega(&Crupier, &Oponente, &b, motor);

		determinaGanador(&Crupier, &Oponente, motor);
	}
}