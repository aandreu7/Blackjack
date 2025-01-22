#include "Partida.h"

#include "Jugador.h"

#include "Motor.h"

Partida::~Partida()
{
	if (baraja != nullptr)
		delete baraja;
	if (oponente != nullptr)
		delete oponente;
	if (crupier != nullptr)
		delete crupier;
	if (motor != nullptr)
		delete motor;
}

void Partida::jugadorGana(Jugador* g, Jugador* p, bool empate)
{
	if (!empate)
	{
		if (typeid(*g) == typeid(Oponente))
			std::cout << "OPONENTE GANA" << std::endl;
		else if (typeid(*g) == typeid(Crupier))
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

void Partida::repartirCartaAMano(Jugador* j)
{
	j->anadirCarta(baraja->repartirCarta());
}

void Partida::mostrarEstadoJuego(bool mostrarCartasCrupier)
{
	if (motor != nullptr)
	{
		oponente->mostrar(motor);
		crupier->mostrar(motor, mostrarCartasCrupier);
	}
		
	motor->sleepGame(mostrarCartasCrupier);
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

opcionJugador Partida::leerJugada(bool doblarPermitido)
{
	ButtonOptions opcion = motor->awaitForInput(oponente, crupier);

	switch (opcion)
	{
	case ButtonOptions::Pedir:
		return opcionJugador::PedirCarta;
	case ButtonOptions::Pasar:
		return opcionJugador::Plantarse;
	case ButtonOptions::Doblar:
		if (doblarPermitido)
			return opcionJugador::Doblar;
	default:
		std::exit(0);
	}
}

void Partida::crupierJuega()
{
	mostrarEstadoJuego(true); // MOSTRAR LA CARTA OCULTA

	while (crupier->getPuntuacion() < 17) // REPARTIR NUEVAS CARTAS
	{
		repartirCartaAMano(crupier);
		mostrarEstadoJuego(true);
	}
}

void Partida::oponenteJuega()
{
	doblarPermitido = false;

	bool stop = false;

	do
	{
		repartirCartaAMano(oponente);
		mostrarEstadoJuego();

		if (oponente->getPuntuacion() >= 21)
			stop = true;
		else
		{
			opcionJugador op = leerJugada(false);
			if (op == opcionJugador::Plantarse)
				stop = true;
		}
	} while (!stop);
}

void Partida::determinaGanador()
{
	mostrarEstadoJuego(true);

	if (oponente->getPuntuacion() > 21) // OPONENTE SE PASA -> CRUPIER GANA
		jugadorGana(crupier, oponente, false);

	else if (crupier->getPuntuacion() > 21) // CRUPIER SE PASA -> OPONENTE GANA
		jugadorGana(oponente, crupier, false);

	else if (oponente->getPuntuacion() > crupier->getPuntuacion()) // OPONENTE GANA
		jugadorGana(oponente, crupier, false);

	else if (oponente->getPuntuacion() < crupier->getPuntuacion()) // CRUPIER GANA
		jugadorGana(crupier, oponente, false);

	else // EMPATE
		jugadorGana(crupier, oponente, true);
}

bool Partida::iniciarJuego(opcionJugador& op)
{
	/*
	DEVUELVE:
	true: JUEGO CONTINUA
	false: JUEGO TERMINADO
	*/

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	baraja->barajar();

	repartirCartaAMano(oponente);
	repartirCartaAMano(oponente);

	repartirCartaAMano(crupier);
	repartirCartaAMano(crupier);

	mostrarEstadoJuego();

	if (oponente->comprovarBlackjack())
	{
		mostrarEstadoJuego(true);
		if (crupier->comprovarBlackjack()) // LOS DOS TIENEN BLACKJACK
			jugadorGana(oponente, crupier, true);
		else
			jugadorGana(oponente, crupier, false);
		return false;
	}
	else if (crupier->comprovarBlackjack())
	{
		mostrarEstadoJuego(true);
		jugadorGana(crupier, oponente, false);
		return false;
	}

	op = leerJugada(true);

	return true;
}

void Partida::jugar()
{
	doblarPermitido = true;

	baraja = new Baraja();

	oponente = new Oponente();
	crupier = new Crupier();

	opcionJugador op;

	if (iniciarJuego(op))
	{
		if (op == opcionJugador::Doblar)
		{
			oponente->setApuesta(oponente->getApuesta() * 2);
			crupier->setApuesta(crupier->getApuesta() * 2);
			repartirCartaAMano(oponente);
			mostrarEstadoJuego();
		}

		else if (op == opcionJugador::PedirCarta)
			oponenteJuega();

		if (oponente->getPuntuacion() <= 21)
			crupierJuega();

		determinaGanador();
	}

	freeMemory(baraja);
	freeMemory(oponente);
	freeMemory(crupier);
}