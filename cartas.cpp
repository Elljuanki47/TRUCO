#include "cartas.h"
#include <algorithm>
#include <random>

/// Asigno la jerarquia real del truco a una carta
int obtenerJerarquia(int valor, Palo palo) {
	if (valor == 1 && palo == ESPADA) return 14;
	if (valor == 1 && palo == BASTO) return 13;
    if (valor == 7 && palo == ESPADA) return 12;
    if (valor == 7 && palo == ORO) return 11;
    if (valor == 3) return 10;
    if (valor == 2) return 9;
    if (valor == 1) return 8;
    if (valor == 12) return 7;
    if (valor == 11) return 6;
    if (valor == 10) return 5;
    if (valor == 7) return 4;
    if (valor == 6) return 3;
    if (valor == 5) return 2;
    return 1; // 4
}

std::vector<Carta> crearMazo() {
	std::vector<Carta> mazo;
	int valores[] = {1,2,3,4,5,6,7,10,11,12};

	for (int p = ESPADA; p <= COPA; p++) {
		for (int v : valores) {
			Carta c;
			c.valor = v;
			c.palo = static_cast<Palo>(p);
			c.jerarquia = obtenerJerarquia(v, c.palo);
			mazo.push_back(c);
		}
	}
	return mazo;
}

void mezclarMazo(std::vector<Carta>& mazo) {
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(mazo.begin(), mazo.end(), g);
}

Carta robarCarta(std::vector<Carta>& mazo) {
	Carta c = mazo.back();
	mazo.pop_back();
	return c;
}

int compararCartas(const Carta& c1, const Carta& c2) {
	if (c1.jerarquia > c2.jerarquia) return 1;
	if (c1.jerarquia < c2.jerarquia) return -1;
	return 0; // empate
}

std::string cartaToString(const Carta& c) {
	std::string palo;

	switch (c.palo) {
		case ESPADA: palo ="Espada"; break;
		case BASTO: palo = "Basto"; break;
		case ORO: palo = "Oro"; break;
		case COPA: palo = "Copa"; break;
	}

	return std::to_string (c.valor) + " de " + palo;
}
