#pragma once

#include "puerto.h"

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