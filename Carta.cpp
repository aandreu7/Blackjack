#include "Carta.h"

std::ostream& operator<<(std::ostream& output, const Carta& carta)
{
	switch (carta.getTipo())
	{
	case Tipo::As:
		output << "As" << " ";
		break;
	case Tipo::J:
		output << "J" << " ";
		break;
	case Tipo::Q:
		output << "Q" << " ";
		break;
	case Tipo::K:
		output << "K" << " ";
		break;
	default:
		output << carta.getValor() << " ";
	}

	switch (carta.getPalo())
	{
	case Palo::Picas:
		output << simbolosPalo::Picas;
		break;
	case Palo::Diamantes:
		output << simbolosPalo::Diamantes;
		break;
	case Palo::Treboles:
		output << simbolosPalo::Treboles;
		break;
	case Palo::Corazones:
		output << simbolosPalo::Corazones;
		break;
	default:
		throw "Tipo de carta inválido";
	}
	output << std::endl;

	return output;
}