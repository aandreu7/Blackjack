#pragma once

#include <SFML/Graphics.hpp>

#include "assert.h"
#include <Windows.h>

#include <filesystem>
namespace fs = std::filesystem;

#include <iostream>
#include <ostream>
#include <locale.h>
#include <ctime>

#include <unordered_map>

#define NUM_PALOS 4
#define NUM_CARTAS 52

namespace simbolosPalo
{
	const char Picas = 'P';
	const char Diamantes = 'D';
	const char Treboles = 'T';
	const char Corazones = 'C';
}

typedef enum
{
	NonePalo = -1,
	Picas,
	Diamantes,
	Treboles,
	Corazones
}Palo;

typedef enum //CUIDADO: NO SON LOS VALORES REALES. PARA EL VALOR REAL HAY QUE SUMAR 1
{
	NoneTipo = -1,
	tAs = 0,
	tX = 1,
	tJ = 2,
	tQ = 3,
	tK = 4
}Tipo;

typedef enum
{
	vJ = 11,
	vQ,
	vK,
	vAs
}ValorTipo;

enum class opcionJugador
{
	PedirCarta,
	Plantarse,
	Doblar
};

enum class rolJugador
{
	None,
	Oponente,
	Crupier
};