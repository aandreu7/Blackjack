#pragma once

#include "Carta.h"

class Motor;

class Jugador
{
protected:
	Carta* cartasJugador;
	int nCartas;
	int puntuacion;
	int apuesta;
	int banca;

public:
	Jugador() { puntuacion = 0; cartasJugador = nullptr; nCartas = 0; apuesta = 0; banca = 0; }
	Jugador(Carta carta1, Carta carta2);
	virtual ~Jugador() { delete[] cartasJugador; }

	// GETTERS
	Carta* getCartas() const { return cartasJugador; }
	int getPuntuacion() const { return puntuacion; }
	int getNCartas() const { return nCartas; }
	int getBanca() const { return banca; }
	int getApuesta() const { return apuesta; }

	// SETTERS
	void setBanca(int nBanca) { banca = nBanca; }
	void setApuesta(int nApuesta) { apuesta = nApuesta; }

	void anadirCarta(const Carta carta);
	void actualizarPuntuacion();
	bool comprovarBlackjack() { return puntuacion == 21; }
};

std::ostream& operator<<(std::ostream& output, const Jugador& jugador);

class Oponente : public Jugador
{
public:
	void mostrar(Motor* motor);
};

class Crupier : public Jugador
{
public:
	void mostrar(Motor* motor, bool mostrarSegundaCarta);
};

