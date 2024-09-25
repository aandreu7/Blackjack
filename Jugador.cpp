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
		if (cartasJugador[i].getTipo() == Tipo::As)
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
	{
		nuevoCartasJugador[i] = cartasJugador[i];
	}
	nuevoCartasJugador[nCartas] = carta;
	nCartas++;
	delete[] cartasJugador;
	cartasJugador = nuevoCartasJugador;
	nuevoCartasJugador = nullptr;
	actualizarPuntuacion();
}