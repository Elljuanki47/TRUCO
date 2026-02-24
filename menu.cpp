#include <iostream>
#include "menu.h"
#include "juego.h"
#include "reglas.h"

using namespace std;

Reglas configurarReglas() {
	Reglas reglas = reglasPorDefecto();
	int opcion;

	cout << "\n=== CONFIGURACION DE LA PARTIDA ===\n";

	/// FLOR
	cout << "¿Jugar con flor?\n";
	cout << "1 - No(por defecto)\n";
	cout << "2 - Si\n";
	cout << "Opcion: ";
	cin >> opcion;

	if (opcion == 2) {
		reglas.flor = true;
	}

	/// PUNTOS
    cout << "\n¿Cuales son los putos maximos?\n";
    cout << "1 - 30 puntos (por defecto)\n";
    cout << "2 - 15 puntos\n";
    cout << "Opción: ";
    cin >> opcion;

    if (opcion == 2) {
        reglas.puntosMax = 15;
    }

    return reglas;
}

void mostrarMenu() {

	/// VARIBALES
	int opcion;
	Reglas reglas;

	/// DESARROLLO
	do {
		cout << "==========================" << endl;
		cout << "      	   TRUCO     " << endl;
		cout << "==========================" << endl;
		cout << "1 - Jugar" << endl;
		cout << "2 - Reglas" << endl;
		cout << "0 - Salir" << endl;
		cout << "Opcion: ";
		cin >> opcion;

		switch (opcion) {
			case 1:
				reglas = configurarReglas();
				cout << "Iniciando juego..." << endl;
				jugarPartida(reglas);
				break;
			case 2:
				cout << R"(
  REGLAS BASICAS DEL TRUCO

El Truco se juega entre dos jugadores utilizando una baraja española de 40 cartas.
Cada ronda se juega al mejor de tres manos.

======================================================

  VALOR DE LAS CARTAS (de mayor a menor)

1 de Espada
1 de Basto
7 de Espada
7 de Oro
3
2
1 (Oro y Copa)
12
11
10
7 (Copa y Basto)
6
5
4

======================================================

  DESARROLLO DE LA RONDA

Cada jugador recibe 3 cartas.

El jugador mano comienza la ronda.

En cada mano, gana quien tire la carta de mayor jerarquía.

El que gana una mano, comienza la siguiente.

======================================================

   EMPATES (PARDA)

Si la primera mano es parda, no define la ronda.

Si hay parda en la segunda o tercera mano, gana quien haya ganado la primera.

Si la primera también fue parda, gana el mano.

======================================================

   TRUCO, RETRUCO Y VALE 4

El Truco puede cantarse antes de tirar una carta.

Si se acepta:

Truco vale 2 puntos

Retruco vale 3 puntos

Vale 4 vale 4 puntos

Si un canto no es aceptado, el rival gana los puntos vigentes.

======================================================

   ENVIDO Y FLOR

El Envido se canta antes de la primera mano.

Se calcula sumando dos cartas del mismo palo más 20.

Las cartas 10, 11 y 12 valen 0 para el Envido.

La Flor solo se juega si está habilitada en la configuración.

======================================================

   OBJETIVO

Gana la partida quien alcance primero el puntaje máximo configurado
(15 o 30 puntos))";
				break;
			case 0:
				cout << "Saliendo del juego..." << endl;

			default:
				cout << "Opcion inválida" << endl;
			}

		cout << endl;

	} while (opcion != 0);
}

