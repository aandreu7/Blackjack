#include "Carta.h"

std::ostream& operator<<(std::ostream& output, const Carta& carta)
{
	switch (carta.getTipo())
	{
	case Tipo::tAs:
		output << "As" << " ";
		break;
	case Tipo::tJ:
		output << "J" << " ";
		break;
	case Tipo::tQ:
		output << "Q" << " ";
		break;
	case Tipo::tK:
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