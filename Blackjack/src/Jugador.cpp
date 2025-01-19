#include "Jugador.h"

Jugador::Jugador(Carta carta1, Carta carta2)
{
	cartasJugador = new Carta[2];
	cartasJugador[0] = carta1;
	cartasJugador[1] = carta2;
	nCartas = 2;
	actualizarPuntuacion();
}

void Jugador::actualizarPuntuacion()
{
	int punt = 0;
	bool hayAs = false;
	for (int i = 0; i < nCartas; i++)
	{
		if (cartasJugador[i].getTipo() == Tipo::tAs)
			hayAs = true;
		punt += cartasJugador[i].getValor();
	}
	if (hayAs && punt>21)
		punt -= 10;
	puntuacion = punt;
}

std::ostream& operator<<(std::ostream& output, const Jugador& jugador)
{
	for (int i = 0; i < jugador.getNCartas(); i++)
	{
		output << jugador.getCartas()[i];
	}
	output << "PUNTUACION: " << jugador.getPuntuacion() << std::endl;
	return output;
}

void Jugador::anadirCarta(const Carta carta)
{
	Carta* nuevoCartasJugador = new Carta[nCartas + 1];
	for (int i = 0; i < nCartas; i++)
		nuevoCartasJugador[i] = cartasJugador[i];
	nuevoCartasJugador[nCartas] = carta;
	nCartas++;
	delete[] cartasJugador;
	cartasJugador = nuevoCartasJugador;
	nuevoCartasJugador = nullptr;
	actualizarPuntuacion();
}

void Jugador::mostrar(Motor* motor)
{
	float xPos = 50.0f;
	float yPos = 20.0f;

	if (rol == rolJugador::Oponente)
		yPos = 1200.f;
	

	for (int i = 0; i < nCartas; i++)
	{
		int cardValue = cartasJugador[i].getValor();

		switch (cartasJugador[i].getTipo())
		{
		case Tipo::tX:
			break;
		case Tipo::tJ:
			cardValue = ValorTipo::vJ;
			break;
		case Tipo::tQ:
			cardValue = ValorTipo::vQ;
			break;
		case Tipo::tK:
			cardValue = ValorTipo::vK;
			break;
		case Tipo::tAs:
			cardValue = ValorTipo::vAs;
			break;
		default:
			throw ("Invalid card value");
		}

		sf::Sprite* sprite = motor->getCardMap()[motor->getCardKey(cardValue, cartasJugador[i].getPalo())];

		sprite->setPosition(xPos, yPos);

		motor->getWindow().draw(*sprite);

		xPos += sprite->getGlobalBounds().width + 10.f;
	}
}
