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

    Partida() { motor = nullptr; }
    Partida(Motor* nMotor) { motor = nMotor; }

    ~Partida();

    void jugar();

    void apuestas();

    bool doblarPermitido;

private:

    Baraja* baraja;
    Oponente* oponente;
    Crupier* crupier;
    Motor* motor;

    void jugadorGana(Jugador* ganador, Jugador* perdedor, bool empate);
    void repartirCartaAMano(Jugador* jugador);
    void mostrarEstadoJuego(bool mostrarCartasCrupier = false);
    void mostrarOpciones(bool doblarPermitido);
    opcionJugador leerJugada(bool doblarPermitido);
    void crupierJuega();
    void oponenteJuega();
    void determinaGanador();
    bool iniciarJuego(opcionJugador& op);
};