#pragma once
#include "Carta.h"

class Jugador
{
private:
	Carta* cartasJugador;
	int nCartas;
	int puntuacion;
public:
	Jugador() { puntuacion = 0; cartasJugador = nullptr; nCartas = 0; }
	Jugador(Carta carta1, Carta carta2);
	~Jugador() { ; }
	Carta* getCartas() const { return cartasJugador; }
	int getPuntuacion() const { return puntuacion; }
	int getNCartas() const { return nCartas; }
	void anadirCarta(const Carta carta);
	void actualizarPuntuacion();
};

std::ostream& operator<<(std::ostream& output, const Jugador& jugador);

