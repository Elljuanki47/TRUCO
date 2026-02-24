#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <vector>
#include "cartas.h"
#include "reglas.h"
#include "envido.h"

/// PARTIDA
void jugarPartida(const Reglas& reglas);
void jugarRonda(int& puntosJugador,
				int& puntosCPU,
				const Reglas& reglas,
				bool jugadorEsMano);

/// CARTAS
int pedirCartaValida(int max);
int elegirCartaCPU(const std::vector<Carta>& manoCPU);

/// TRUCO
bool gestionarTruco(int& nivelTruco,
					int& valorMano,
					bool puedeCantarJugador,
					const std::vector<Carta>& manoCPU,
					bool& ganadorPorNoQuieroEsJugador);

/// ENVIDO
int calcularEnvido(const std::vector<Carta>& mano);

void gestionarEnvido(const std::vector<Carta>& manoJugador,
					 const std::vector<Carta>& manoCPU,
					 int& puntosJugador,
					 int& puntosCPU,
					 const Reglas& reglas,
					 bool jugadorEsMano);

int resolverEnvido(
	const std::vector<Carta>& manoJugador,
	const std::vector<Carta>& manoCPU);

int resolverFlor(
	const std::vector<Carta>& manoJugador,
	const std::vector<Carta>& manoCPU);

#endif // JUEGO_H_INCLUDED
