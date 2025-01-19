#pragma once

#include "puerto.h"

class Carta
{
private:
	int valor;
	Palo palo;
	Tipo tipo;
	int id; // ASSOCIATES CARD WITH TEXTURE
public:
	Carta() { valor = -1; palo = Palo::NonePalo, tipo = Tipo::NoneTipo; id = -1; }
	Carta(int nValor, Palo nPalo, Tipo nTipo, int nId) : valor(nValor), palo(nPalo), tipo(nTipo), id(nId) { ; }
	~Carta() { ; } 
	int getValor() const { return valor; }
	Palo getPalo() const { return palo; }
	Tipo getTipo() const { return tipo; }
	void setValor(int nValor) { valor = nValor; }
	void setPalo(Palo nPalo) { palo = nPalo; }
	void setTipo(Tipo nTipo) { tipo = nTipo; }
};

std::ostream& operator<<(std::ostream& output, const Carta& carta);