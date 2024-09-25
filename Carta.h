#pragma once
#include <iostream>
#include <ostream>
#include <locale.h>
#include <ctime>
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
	NonePalo=-1,
	Picas,
	Diamantes,
	Treboles,
	Corazones
}Palo;

typedef enum //CUIDADO: NO SON LOS VALORES REALES. PARA EL VALOR REAL HAY QUE SUMAR 1
{
	NoneTipo = -1,
	As = 0,
	X = 1,
	J = 2,
	Q = 3,
	K = 4
}Tipo;

class Carta
{
private:
	int valor;
	Palo palo;
	Tipo tipo;
public:
	Carta() { valor = -1; palo = Palo::NonePalo, tipo = Tipo::NoneTipo; }
	Carta(int nValor, Palo nPalo, Tipo nTipo) : valor(nValor), palo(nPalo), tipo(nTipo) { ; }
	~Carta() { ; } 
	int getValor() const { return valor; }
	Palo getPalo() const { return palo; }
	Tipo getTipo() const { return tipo; }
	void setValor(int nValor) { valor = nValor; }
	void setPalo(Palo nPalo) { palo = nPalo; }
	void setTipo(Tipo nTipo) { tipo = nTipo; }
};

std::ostream& operator<<(std::ostream& output, const Carta& carta);