#pragma once

#include "puerto.h"
#include "Carta.h"
#include "Motor.h"

class Motor;

class Jugador
{
private:
	Carta* cartasJugador;
	int nCartas;
	int puntuacion;
	float apuesta;
	rolJugador rol;
	float banca;
public:
	Jugador() { puntuacion = 0; cartasJugador = nullptr; nCartas = 0; apuesta = 0; rol = rolJugador::None; banca = 0; }
	Jugador(rolJugador nRol) : rol(nRol) { puntuacion = 0; cartasJugador = nullptr; nCartas = 0; apuesta = 0; banca = 0; }
	Jugador(Carta carta1, Carta carta2);
	~Jugador() { ; }

	// GETTERS
	Carta* getCartas() const { return cartasJugador; }
	int getPuntuacion() const { return puntuacion; }
	int getNCartas() const { return nCartas; }
	rolJugador getRol() const { return rol; }
	float getBanca() const { return banca; }
	float getApuesta() const { return apuesta; }

	// SETTERS
	void setBanca(float nBanca) { banca = nBanca; }
	void setApuesta(float nApuesta) { apuesta = nApuesta; }

	void anadirCarta(const Carta carta);
	void actualizarPuntuacion();
	bool comprovarBlackjack() { return puntuacion == 21; }

	void mostrar(Motor* motor);
};

std::ostream& operator<<(std::ostream& output, const Jugador& jugador);

