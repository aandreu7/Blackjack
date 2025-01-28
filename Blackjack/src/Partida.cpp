#include "Partida.h"

#include "Jugador.h"

#include "Motor.h"

Partida::Partida(Motor* nMotor)
{
	motor = nMotor;

	bote = 0;
	doblarPermitido = false;
	juegoEmpezado = false;
}

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

void Partida::readData()
{
	if (crupier == nullptr || oponente == nullptr)
		return;

	std::ifstream archivo("./game_data/game_data.json");
	if (!archivo.is_open()) {
		std::cerr << "No se pudo abrir el archivo JSON.\n";
		return;
	}

	// Parsear el archivo JSON
	nlohmann::json datos;
	archivo >> datos;

	// Leer los valores
	int bancaCrupier = datos["juego"]["crupier"];
	int bancaOponente = datos["juego"]["oponente"];

	if (bancaCrupier > 0 && bancaOponente > 0)
	{
		crupier->setBanca(bancaCrupier);
		oponente->setBanca(bancaOponente);
	}

	else
	{
		crupier->setBanca(1000);
		oponente->setBanca(1000);
	}
}

void Partida::writeData()
{
	if (crupier == nullptr || oponente == nullptr)
		return;

	// Crear el objeto JSON
	nlohmann::json datos;
	datos["juego"]["crupier"] = crupier->getBanca();
	datos["juego"]["oponente"] = oponente->getBanca();

	// Abrir el archivo JSON para escribir
	std::ofstream archivo("./game_data/game_data.json");
	if (!archivo.is_open()) {
		std::cerr << "No se pudo abrir el archivo JSON para escribir.\n";
		return;
	}

	// Escribir los datos en el archivo
	archivo << datos.dump(4); // Formatear con 4 espacios de indentación para hacerlo legible
	archivo.close();
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

		g->setBanca(g->getBanca() + bote);
	}
	else
	{
		std::cout << "EMPATE" << std::endl;
		g->setBanca(g->getBanca() + bote / 2);
		p->setBanca(p->getBanca() + bote / 2);
	}

	g->setApuesta(0);
	p->setApuesta(0);
}

void Partida::repartirCartaAMano(Jugador* j)
{
	j->anadirCarta(baraja->repartirCarta());
}

void Partida::mostrarEstadoJuego(bool mostrarCartasCrupier, bool sleep)
{
	if (motor != nullptr)
	{
		oponente->mostrar(motor);
		crupier->mostrar(motor, mostrarCartasCrupier);
		motor->showPot(bote);
	}
	
	if (sleep)
		motor->sleepGame(mostrarCartasCrupier);
}

opcionJugador Partida::leerJugada()
{
	ButtonOptions opcion = motor->awaitForInput(false, oponente, crupier);

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
			opcionJugador op = leerJugada();
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

	op = leerJugada();

	return true;
}

void Partida::apuestasIniciales()
{
	int apuestaActSeleccionada = 0;
	int totalApostado = 0;

	ButtonOptions opcion = ButtonOptions::None;

	do
	{
		opcion = motor->awaitForInput(true, oponente, crupier, totalApostado, apuestaActSeleccionada);

		switch (opcion)
		{
		case ButtonOptions::Subir:
			if (apuestaActSeleccionada < oponente->getBanca() && apuestaActSeleccionada < crupier->getBanca())
				apuestaActSeleccionada = apuestaActSeleccionada + 10;
			break;
		case ButtonOptions::Bajar:
			if (apuestaActSeleccionada > 0)
				apuestaActSeleccionada = apuestaActSeleccionada - 10;
			break;
		case ButtonOptions::Apostar:
			if (oponente->getBanca() >= apuestaActSeleccionada && crupier->getBanca() >= apuestaActSeleccionada)
			{
				oponente->setApuesta(oponente->getApuesta() + apuestaActSeleccionada);
				crupier->setApuesta(crupier->getApuesta() + apuestaActSeleccionada);
				totalApostado += apuestaActSeleccionada;
				oponente->setBanca(oponente->getBanca() - apuestaActSeleccionada);
				crupier->setBanca(crupier->getBanca() - apuestaActSeleccionada);
			}
			break;
		default:
			break;
		}
	} while (opcion != ButtonOptions::Empezar || totalApostado <= 0);

	bote = totalApostado * 2;
}

void Partida::jugar()
{
	baraja = new Baraja();

	oponente = new Oponente();
	crupier = new Crupier();

	readData();

	apuestasIniciales();

	if (oponente->getBanca() >= oponente->getApuesta() && crupier->getBanca() >= crupier->getApuesta())
		doblarPermitido = true;
	else
		doblarPermitido = false;

	opcionJugador op;

	if (iniciarJuego(op))
	{
		if (op == opcionJugador::Doblar)
		{
			oponente->setBanca(oponente->getBanca() - oponente->getApuesta());
			oponente->setApuesta(oponente->getApuesta() * 2);
			crupier->setBanca(crupier->getBanca() - crupier->getApuesta());
			crupier->setApuesta(crupier->getApuesta() * 2);
			bote *= 2;
			repartirCartaAMano(oponente);
			mostrarEstadoJuego();
		}

		else if (op == opcionJugador::PedirCarta)
			oponenteJuega();

		if (oponente->getPuntuacion() <= 21)
			crupierJuega();

		determinaGanador();
	}

	writeData();

	freeMemory(baraja);
	freeMemory(oponente);
	freeMemory(crupier);
}