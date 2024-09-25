#include "Baraja.h"

Baraja::Baraja()
{
	cartas = new Carta[NUM_CARTAS];
	for (int iPalo = 0; iPalo < NUM_PALOS; iPalo++)
	{
		for (int iCarta = 0; iCarta < NUM_CARTAS / NUM_PALOS; iCarta++)
		{
			Tipo tipo = NoneTipo;
			int valor;
			switch (iCarta)
			{
			case 0:
				tipo = As;
				valor = 11;
				break;
			case 10:
				tipo = J;
				valor = 10;
				break;
			case 11:
				tipo = Q;
				valor = 10;
				break;
			case 12:
				tipo = K;
				valor = 10;
				break;
			default:
				tipo = X;
				valor = iCarta + 1;
			}
			Carta* nCarta = new Carta(valor, Palo(iPalo), tipo);
			cartas[iPalo * NUM_CARTAS / NUM_PALOS + iCarta] = *nCarta;
		}
	}
	numCartas = NUM_CARTAS;
}

int numeroRandom(int max)
{
	int min = 0;
	return rand() % (max - min) + min;
}

void Baraja::barajar()
{
	int az, tmp;
	for (int k = numCartas - 1; k > 0; k--)
	{
		az = numeroRandom(k);
		Carta tmp = cartas[az];
		cartas[az] = cartas[k];
		cartas[k] = tmp;
	}

}

Carta Baraja::repartirCarta()
{
	Carta* nuevaBaraja = new Carta[numCartas-1];
	Carta cartaSeleccionada = cartas[0];
	for (int i = 1; i < numCartas; i++)
	{
		nuevaBaraja[i-1] = cartas[i];
	}
	delete[] cartas;
	cartas = nuevaBaraja;
	nuevaBaraja = nullptr;
	numCartas--;
	return cartaSeleccionada;
}

std::ostream& operator<<(std::ostream& os, const Baraja& b)
{
	for (int i = 0; i < b.numCartas; i++)
	{
		os << b.cartas[i];
	}
	return os;
}