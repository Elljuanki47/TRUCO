#ifndef CARTAS_H_INCLUDED
#define CARTAS_H_INCLUDED

#include <string>
#include <vector>

enum Palo {
	ESPADA,
	BASTO,
	ORO,
	COPA
};

struct Carta {
	int valor;
	Palo palo;
	int jerarquia;
};

/// Funciones relacionadas con las cartas
std::vector<Carta> crearMazo();
void mezclarMazo(std::vector<Carta>& mazo);
Carta robarCarta(std::vector<Carta>& mazo);
int compararCartas(const Carta& c1, const Carta& c2);
std::string cartaToString(const Carta& c);

#endif // CARTAS_H_INCLUDED
