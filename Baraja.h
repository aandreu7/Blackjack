#pragma once
#include "Carta.h"
class Baraja
{
private:
	Carta* cartas;
	int numCartas;
public:
	Baraja();
	~Baraja() { ; }
	void barajar();
	int getNumCartas() { return numCartas; }
	Carta repartirCarta();

	friend std::ostream& operator<<(std::ostream& output, const Baraja& b);
};
