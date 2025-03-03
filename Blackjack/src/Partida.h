#pragma once

#include "Baraja.h"

class Jugador;

class Oponente;
class Crupier;

class Motor;

class Partida 
{
    friend class Motor;

public:

    Partida(Motor* nMotor = nullptr);

    ~Partida();

    void jugar();

    void apuestasIniciales();

    void setDimApuesta(int nDimApuesta) { dimApuesta = nDimApuesta; }

	int getBote() const { return bote; }

    bool doblarPermitido;

    bool juegoEmpezado;

private:

    Baraja* baraja;
    Oponente* oponente;
    Crupier* crupier;
    Motor* motor;

    int bote;

    int dimApuesta;

    void jugadorGana(Jugador* ganador, Jugador* perdedor, bool empate);
    void repartirCartaAMano(Jugador* jugador, bool playSound=true);
    void mostrarEstadoJuego(bool mostrarCartasCrupier = false, bool sleep = true);
    opcionJugador leerJugada();
    void crupierJuega();
    void oponenteJuega();
    void determinaGanador();
    bool iniciarJuego(opcionJugador& op);
    void readData();
    void writeData();
};