#include "Jugador.h"

#include "Motor.h"

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
	int nAses = 0;

	for (int i = 0; i < nCartas; i++)
	{
		punt += cartasJugador[i].getValor();
		if (cartasJugador[i].getTipo() == Tipo::tAs)
			nAses++;
	}

	while (punt > 21 && nAses > 0)
	{
		punt -= 10;
		nAses--;
	}

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

float mostrarAux(float posX, float posY, Carta* cartas, Motor* motor, int i)
{
	int cardValue = cartas[i].getValor();

	switch (cartas[i].getTipo())
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

	return motor->showCard(motor->getCardKey(cardValue, cartas[i].getPalo()), posX, posY);
}

void Oponente::mostrar(Motor* motor)
{
	float xPos = motor->getWidth() * 0.05;
	float yPos = motor->getHeight() * 0.7;

	for (int i = 0; i < nCartas; i++)
		xPos += mostrarAux(xPos, yPos, cartasJugador, motor, i) + 0.01 * motor->getWidth();

	if (puntuacion > 0)
		motor->showText("TU PUNTUACIÓN: " + std::to_string(puntuacion), xPos, yPos * 1.25);
	motor->showText("TU BANCA: " + std::to_string(banca), motor->getWidth() * 0.9, motor->getHeight() * 0.2);
}

void Crupier::mostrar(Motor* motor, bool mostrarSegundaCarta)
{
	float xPos = motor->getWidth() * 0.05;
	float yPos = motor->getHeight() * 0.05;

	for (int i = 0; i < nCartas; i++)
	{

		// Crupier does not show the second card
		if (nCartas == 2 && i == 1 && !segundaCartaYaMostrada)
		{
			if (!mostrarSegundaCarta)
			{
				motor->showCard(0, xPos, yPos);
				break;
			}
			else
			{
				motor->playSound(0);
				segundaCartaYaMostrada = true;
			}				
		}

		xPos += mostrarAux(xPos, yPos, cartasJugador, motor, i) + 0.01 * motor->getWidth();
	}

	if (mostrarSegundaCarta)
		motor->showText("PUNTUACION CRUPIER: " + std::to_string(puntuacion), xPos, yPos * 1.25);
	motor->showText("BANCA CRUPIER: " + std::to_string(banca), motor->getWidth() * 0.9, motor->getHeight() * 0.1);
}