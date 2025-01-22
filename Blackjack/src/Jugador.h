#pragma once

#include "Carta.h"

class Motor;

class Jugador
{
protected:
	Carta* cartasJugador;
	int nCartas;
	int puntuacion;
	float apuesta;
	float banca;

public:
	Jugador() { puntuacion = 0; cartasJugador = nullptr; nCartas = 0; apuesta = 0; banca = 0; }
	Jugador(Carta carta1, Carta carta2);
	virtual ~Jugador() { delete[] cartasJugador; }

	// GETTERS
	Carta* getCartas() const { return cartasJugador; }
	int getPuntuacion() const { return puntuacion; }
	int getNCartas() const { return nCartas; }
	float getBanca() const { return banca; }
	float getApuesta() const { return apuesta; }

	// SETTERS
	void setBanca(float nBanca) { banca = nBanca; }
	void setApuesta(float nApuesta) { apuesta = nApuesta; }

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

