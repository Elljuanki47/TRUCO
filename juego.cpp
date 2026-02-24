#include <iostream>
#include "juego.h"
#include "cartas.h"
#include <vector>
#include "reglas.h"
#include "envido.h"

using namespace std;

int pedirCartaValida(int max) {
	int opcion;

	while(true) {
		cout << "Elegi una carta (1 a " << max << "): ";
		cin >> opcion;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Entrada invalida. Intente nuevamente.\n";
			continue;
		}

		if (opcion >= 1 && opcion <=max) {
			return opcion -1;
		}

		cout << "Opcion fuera de rango. Intente nuevamente.\n";
	}
}

/// ==========================================================================================================================================

int elegirCartaCPU(const vector<Carta>& manoCPU) {
	int indiceMenor = 0;

	for(size_t i = 1; i < manoCPU.size(); i++) {
		if (manoCPU[i].jerarquia < manoCPU[indiceMenor].jerarquia) {
			indiceMenor = i;
		}
	}

	return indiceMenor;
}

/// ==========================================================================================================================================

static int fuerzaTrucoCPU(const std::vector<Carta>& manoCPU) {
	int best = manoCPU[0].jerarquia;
	int second = -1;

	for (size_t i = 1; i < manoCPU.size(); i++) {
		int j = manoCPU[i].jerarquia;
		if (j > best) {
			second = best;
			best = j;
		} else if(j > second) {
			second = j;
		}
	}

	int score = 0;

	// Mejor carta
	if (best >= 12) score += 4;			// 1 espada/basto o 7 espada/7 oro
	else if (best >= 9) score += 3;		// 3 o 2
	else if (best >= 8) score += 3;		// ancho falso
	else if (best >= 6) score += 2;		// 12 u 11
	else score += 1;

	//Segunda mejor carta
	if (second >= 10) score += 2; 		// 3
	else if (second >= 8) score += 2;	// 2 o ancho falso
	else if (second >= 6) score += 1;	// 12 / 11
	// resto: 0

	return score;
}

/// ==========================================================================================================================================

static bool cpuAceptaTruco(const std::vector<Carta>& manoCPU, int nivelTrucoActual) {
	int f = fuerzaTrucoCPU(manoCPU);

	// nivelTrucoActual indica que se le esta pidiendo aceptar
	// 0 truco
	// 1 retruco
	// 2 vale 4

	if (nivelTrucoActual == 0) return f >= 3;
	if (nivelTrucoActual == 1) return f >= 4;
	if (nivelTrucoActual == 2) return f >= 5;
	return true;
}

/// ==========================================================================================================================================

static bool cpuQuiereCantarTruco(const std::vector<Carta>& manoCPU, int nivelTrucoActual) {
	// CPU canta si esta por tirar y su mano es fuerte
	int f = fuerzaTrucoCPU(manoCPU);

	if (nivelTrucoActual == 0) return f>= 4;	//Cantar truco
	if (nivelTrucoActual == 1) return f>= 5;	//Cantar retruco
	if (nivelTrucoActual == 2) return f>= 6;	//Cantar vale 4
	return false;
}

/// ==========================================================================================================================================

bool gestionarTruco(int& nivelTruco,
					int& valorMano,
					bool puedeCantar,
					bool cantaCPU,
					const std::vector<Carta>& manoCPU,
					bool& ganadorPorNoQuieroEsJugador) {

	ganadorPorNoQuieroEsJugador = false;

	// Opción A: solo puede cantar el que está por tirar
	if (!puedeCantar) return true;

	// Ya está en Vale 4, no se puede subir más
	if (nivelTruco >= 3) return true;

	// 1) Determinar si se canta
	if (!cantaCPU) {
		// Jugador por menú
		int op;
		cout << "\n¿Queres cantar?\n";
		if (nivelTruco == 0) cout << "1 - Truco\n";
		else if (nivelTruco == 1) cout << "1 - Retruco\n";
		else if (nivelTruco == 2) cout << "1 - Vale 4\n";
		cout << "0 - No cantar\n";
		cout << "Opción: ";
		cin >> op;

		if (op == 0) return true;

		if (nivelTruco == 0) cout << "Vos: Truco\n";
		else if (nivelTruco == 1) cout << "Vos: Retruco\n";
		else cout << "Vos: Vale 4\n";
	} else {
		// CPU decide si canta
		if (!cpuQuiereCantarTruco(manoCPU, nivelTruco)) return true;

		if (nivelTruco == 0) cout << "\nCPU: Truco\n";
		else if (nivelTruco == 1) cout << "\nCPU: Retruco\n";
		else cout << "\nCPU: Vale 4\n";
	}

	// 2) Respuesta del rival
	if (!cantaCPU) {
		// CPU responde al canto del jugador
		if (!cpuAceptaTruco(manoCPU, nivelTruco)) {
			cout << "CPU: No quiero\n";
			// Puntos por “No quiero” = valor anterior: 1 / 2 / 3
			valorMano = (nivelTruco == 0) ? 1 : (nivelTruco == 1) ? 2 : 3;
			ganadorPorNoQuieroEsJugador = true;
			return false;
		}
		cout << "CPU: Quiero\n";
	} else {
		// Jugador responde al canto de la CPU
		int q;
		cout << "¿Aceptás?\n1 - Quiero\n0 - No quiero\nOpción: ";
		cin >> q;

		if (q == 0) {
			cout << "Vos: No quiero\n";
			valorMano = (nivelTruco == 0) ? 1 : (nivelTruco == 1) ? 2 : 3;
			ganadorPorNoQuieroEsJugador = false; // gana CPU
			return false;
		}
		cout << "Vos: Quiero\n";
	}

	// 3) Se aceptó: sube nivel y valor
	nivelTruco++;
	valorMano = (nivelTruco == 1) ? 2 : (nivelTruco == 2) ? 3 : 4;

	return true;
}

/// ==========================================================================================================================================

int valorEnvidoCarta(int valor) {
	if (valor >= 10) return 0;
	return valor;
}

/// ==========================================================================================================================================

int valorFlorCarta(const Carta& c) {
	if (c.valor >= 10) {
		return 0;
	}
	return c.valor;
}

/// ==========================================================================================================================================

bool tieneFlor(const std::vector<Carta>& mano) {
	if (mano.size()!= 3) return false;

	const Palo palo = mano[0].palo;

	return 	mano[1].palo == palo &&
			mano[2].palo == palo;
}

/// ==========================================================================================================================================

int calcularFlor(const std::vector<Carta>& mano) {
	int total = 20;

	for (const Carta& c : mano) {
		total += valorFlorCarta(c);
	}

	return total;
}

/// ==========================================================================================================================================

int calcularEnvido(const vector<Carta>& mano) {
	int maxEnvido = 0;

	for (int i = 0; i < mano.size(); i++) {
		for (int j = i + 1; j < mano.size(); j++) {
			if (mano[i].palo == mano[j].palo) {
				int envido = valorEnvidoCarta(mano[i].valor) +
							 valorEnvidoCarta(mano[j].valor) + 20;
				if (envido > maxEnvido)
					maxEnvido = envido;
			}
		}
	}

	if (maxEnvido == 0) {
		for (const Carta& c : mano) {
			int val = valorEnvidoCarta(c.valor);
			if (val > maxEnvido)
				maxEnvido = val;
		}
	}

	return maxEnvido;
}

/// ==========================================================================================================================================

static bool cpuAceptaSegunCanto(int tantoCPU, int cantEnvido, bool cantoReal, bool cantoFalta) {
	// Envido >23 => >= 24
	// Envido Envido >27 => >= 28
	// real envido >29 => >= 30
	// Envido + Real >30 => >= 31
	// Falta >32 => >=33
	// Envido + Falta = 33

	if (cantoFalta){
		// Falta o Envido + Falta
		return (tantoCPU >= 33);
	}

	if (cantoReal && cantEnvido >= 1) {			// Envido + Real
		return (tantoCPU >= 31);
	}
	if (cantoReal) {							// Real solo
		return (tantoCPU >= 30);
	}

	if (cantEnvido >= 2) {						// Envido + Envido
		return (tantoCPU >= 28);
	}

	if (cantEnvido == 1) {						// Envido solo
		return (tantoCPU >= 24);
	}

	return false;
}

/// ==========================================================================================================================================

static int calcularFaltaEnvido(int puntosJugador, int puntosCPU, const Reglas& reglas) {
	int mayor = (puntosJugador > puntosCPU) ? puntosJugador : puntosCPU;
	return reglas.puntosMax - mayor;
}

/// ==========================================================================================================================================

void gestionarEnvido(const std::vector<Carta>& manoJugador,
					 const std::vector<Carta>& manoCPU,
					 int& puntosJugador,
					 int& puntosCPU,
					 const Reglas& reglas,
					 bool jugadorEsMano) {

	// Calcular tantos
	int tantoJugador = calcularEnvido(manoJugador);
	int tantoCPU = calcularEnvido(manoCPU);

	// Variables del canto acumulado
	int puntosSiQuieren = 0; // Lo que vale si acepta el ultimo canto
	int puntosSiNoQuieren = 1; // lo que cobra el que cantó si el rival no quiere el envido

	int cantEnvido = 0; // 0, 1, 2
	bool cantoReal = false;
	bool cantoFalta = false;

	auto aplicarEnvido = [&]() {
		if (puntosSiQuieren > 0) puntosSiNoQuieren = puntosSiQuieren;
		puntosSiQuieren += 2;
		cantEnvido++;
	};
	auto aplicarReal = [&]() {
		if (puntosSiQuieren > 0) puntosSiNoQuieren = puntosSiQuieren;
		puntosSiQuieren += 3;
		cantoReal = true;
	};
	auto aplicarFalta = [&]() {
		if (puntosSiQuieren > 0) puntosSiNoQuieren = puntosSiQuieren;
		puntosSiQuieren = calcularFaltaEnvido(puntosJugador, puntosCPU, reglas);
		cantoFalta = true;
	};

	// Determinar quien canta primero

	bool alguienCanto = false;
	bool quienCantoEsJugador = false;

	// Si CPU es mano decide si canta y que canta
	auto menuJugadorCantar = [&]() {
		int opcion;
		std::cout << "\nQUeres cantar el tanto?\n";
		std::cout << "1 - Envido\n";
		std::cout << "2 - Real envido\n";
		std::cout << "3 - Falta envido\n";
		if (reglas.flor) std::cout << "4 - Flor\n";
		std::cout << "0 - No cantar\n";
		std::cout << "Opcion: ";
		std::cin >> opcion;

		if (opcion == 0) return false;

		if (opcion == 4 && reglas.flor) {
			 //Flor
			 int ganador = resolverFlor(manoJugador, manoCPU);
			 std::cout << "\nSe juega FLOR\n";
			 if (ganador == 1) puntosJugador += 3;
			 else if (ganador == -1) puntosCPU += 3;
			 else {
				if (jugadorEsMano) puntosJugador += 3;
				else puntosCPU += 3;
			 }
			 return true; //Termino la funcion flor
		}

		if (opcion == 1) {std::cout << "\nVos cantas: envido\n"; aplicarEnvido(); }
		else if (opcion == 2) {std::cout << "\nVos cantas: Real envido\n"; aplicarReal(); }
		else if (opcion == 3) {std::cout << "\nVos cantas: Falta envido\n"; aplicarFalta(); }
		else return false;

		alguienCanto = true;
		quienCantoEsJugador = true;
		return false;
		};
		//Si la CPU es mano, la cpu tiene la primera chance
		if (!jugadorEsMano) {
			if (tantoCPU >= 33) {std::cout << "\nCPU canta: Falta envido\n"; aplicarFalta(); alguienCanto = true; quienCantoEsJugador = false; }
			else if (tantoCPU >= 31) {std::cout << "\nCPU canta: Real envido\n"; aplicarReal(); alguienCanto = true; quienCantoEsJugador = false; }
			else if (tantoCPU >= 28) {std::cout << "\nCPU canta: Envido\n"; aplicarEnvido(); alguienCanto = true; quienCantoEsJugador = false; }
			else if (tantoCPU >= 24) {std::cout << "\nCPU canta: Envido\n"; aplicarEnvido(); alguienCanto = true; quienCantoEsJugador = false; }

			// Si cpu no canto, el jugador puede cantar
			if (!alguienCanto) {
				bool terminoPorFlor = menuJugadorCantar();
				if (terminoPorFlor) return;
				if (!alguienCanto) return;
			}
		}
		else {
			//Jugador es mano al menu directo
			bool terminoPorFlor = menuJugadorCantar();
			if (terminoPorFlor) return;
			if (!alguienCanto) return;
		}



	// Respuesta del rival Quiero / No quiero

	bool aceptado = false;

	if (quienCantoEsJugador) {
		//CPU acepta o no segun el canto actual y su tanto
		bool cpuQuiere = cpuAceptaSegunCanto(tantoCPU, cantEnvido, cantoReal, cantoFalta);

		if (!cpuQuiere) {
			std::cout << "CPU: No quiero\n";
			puntosJugador += puntosSiNoQuieren;
			return;
		}

		std::cout << "CPU: Quiero\n";
		aceptado = true;
	}
	else {
		// Jugador responde al canto de CPU
		int q;
        std::cout << "¿Aceptas?\n";
        std::cout << "1 - Quiero\n";
        std::cout << "0 - No quiero\n";
        std::cout << "Opción: ";
        std::cin >> q;

        if (q == 0) {
			std::cout << "Vos: No quiero\n";
			puntosCPU += puntosSiNoQuieren;
			return;
        }

        std::cout << "Vos: Quiero\n";
        aceptado = true;
	}

	if (!aceptado) return;

	// Resolver tanto y sumar puntos

	std::cout << "\nVos: Quiero " << tantoJugador << "\n";
	if (tantoCPU > tantoJugador) std::cout << "CPU: " << tantoCPU << " son mejores\n";
	else std::cout << "CPU: Son buenas\n";

	int ganador = resolverEnvido(manoJugador, manoCPU);

	if (ganador == 1) {
		puntosJugador += puntosSiQuieren;
	}
	else if (ganador == -1) {
		puntosCPU += puntosSiQuieren;
	}
	else {
		// Empate define mano
		if (jugadorEsMano) puntosJugador += puntosSiQuieren;
		else puntosCPU += puntosSiQuieren;
	}
}

/// ==========================================================================================================================================

int resolverEnvido(const std::vector<Carta>& manoJugador, const std::vector<Carta>& manoCPU) {

	int envidoJugador = calcularEnvido(manoJugador);
	int envidoCPU = calcularEnvido(manoCPU);

	if (envidoJugador > envidoCPU) return 1;
	if (envidoCPU > envidoJugador) return -1;
	return 0; /// ARREGLAR RESOLVER POR QUIEN ES MANO
	}

/// ==========================================================================================================================================

int resolverFlor(const std::vector<Carta>& manoJugador,
				 const std::vector<Carta>& manoCPU) {

	bool jugadorTieneFlor = tieneFlor(manoJugador);
	bool cpuTieneFlor = tieneFlor(manoCPU);

	if (jugadorTieneFlor && !cpuTieneFlor) {
		return 1;
	}
	if (cpuTieneFlor && !jugadorTieneFlor) {
		return -1;
	}
	if (!jugadorTieneFlor && !cpuTieneFlor) {
		return 0;
	}

	// Si ambos tienen flor se compara el valor
	int florJugador = calcularFlor(manoJugador);
	int florCPU = calcularFlor(manoCPU);

	if (florJugador > florCPU) {
		return 1;
	}
	if (florCPU > florJugador) {
		return -1;
	}

	return 0;
	}

/// ==========================================================================================================================================

void jugarRonda(int& puntosJugador, int& puntosCPU, const Reglas& reglas, bool jugadorEsMano) {
	cout << "\n==========================\n";
    cout << "Puntaje parcial -> Vos: " << puntosJugador << " | CPU: " << puntosCPU << "\n";
    cout << "==========================\n";
	int valorMano = 1;
	int nivelTruco = 0;
	int manosJugador = 0;
	int manosCPU = 0;
	int ganadorPrimeraMano = 0;
	int pj0 = puntosJugador;
	int pc0 = puntosCPU;

	bool jugadorTiraPrimero = jugadorEsMano;

	std::vector<Carta> mazo = crearMazo();
	mezclarMazo(mazo);

	std::vector<Carta> manoJugador;
	std::vector<Carta> manoCPU;

	for (int i = 0; i < 3; i++) {
		manoJugador.push_back(robarCarta(mazo));
		manoCPU.push_back(robarCarta(mazo));
	}

	std::cout << "\nTus cartas son:\n";
	for (size_t j = 0; j < manoJugador.size();j++) {
		std::cout << j + 1 << " - " << cartaToString(manoJugador[j]) << std::endl;
	}

	gestionarEnvido(manoJugador,manoCPU, puntosJugador, puntosCPU, reglas, jugadorEsMano);

	int pj1 = puntosJugador;
	int pc1 = puntosCPU;

	for (int i = 0; i < 3; i++) {
		std::cout << "\nMano " << i + 1 << std::endl;
		std::cout << "Tus cartas: " << std::endl;

		for (size_t j = 0; j < manoJugador.size(); j++) {
			std::cout << j + 1 << " - " << cartaToString(manoJugador[j]) << std::endl;
		}

		bool ganadorNoQuieroEsJugador = false;
		bool cantaCPU = !jugadorTiraPrimero;

		bool sigue = gestionarTruco (
			nivelTruco,
			valorMano,
			true,
			cantaCPU,
			manoCPU,
			ganadorNoQuieroEsJugador
		);

		if (!sigue) {
			if (ganadorNoQuieroEsJugador) puntosJugador += valorMano;
			else puntosCPU += valorMano;
			return;
		}

		Carta cartaJugador;
		Carta cartaCPU;

		if (jugadorTiraPrimero) {
			int indiceJugador = pedirCartaValida((int)manoJugador.size());
			cartaJugador = manoJugador[indiceJugador];
			manoJugador.erase(manoJugador.begin() + indiceJugador);

			int indiceCPU = elegirCartaCPU(manoCPU);
			cartaCPU = manoCPU[indiceCPU];
			manoCPU.erase(manoCPU.begin() + indiceCPU);

			std::cout << "CPU Juega: " << cartaToString(cartaCPU) << std::endl;
		}
		else {
			int indiceCPU = elegirCartaCPU(manoCPU);
			cartaCPU = manoCPU[indiceCPU];
			manoCPU.erase(manoCPU.begin() + indiceCPU);

			std::cout << "CPU Juega: " << cartaToString(cartaCPU) << std::endl;

			int indiceJugador = pedirCartaValida((int)manoJugador.size());
			cartaJugador = manoJugador[indiceJugador];
			manoJugador.erase(manoJugador.begin() + indiceJugador);
		}

		int resultado = compararCartas(cartaJugador, cartaCPU);

		if (resultado == 1) {
			std::cout << "Ganas la mano" << std::endl;
			manosJugador++;
			jugadorTiraPrimero = true;

			if (i == 0) {
				ganadorPrimeraMano = 1;
			}
		}
		else if (resultado == -1) {
			std::cout << "CPU gana la mano" << std::endl;
			manosCPU++;
			jugadorTiraPrimero = false;

			if (i == 0) {
				ganadorPrimeraMano = -1;
			}
		}
		else {
			std::cout << "Parda" << std::endl;

			if (i == 0) {
				ganadorPrimeraMano = 0;
				jugadorTiraPrimero = jugadorEsMano;
			}
			else {
				if (ganadorPrimeraMano == 1 ||
					(ganadorPrimeraMano == 0 && jugadorEsMano)) {
						manosJugador++;
						jugadorTiraPrimero = true;
					}
					else {
						manosCPU++;
						jugadorTiraPrimero = false;
					}
					break;
			}
		}

		if (manosJugador == 2 || manosCPU == 2) break;
	}

	if (manosJugador > manosCPU) {
		std::cout << "\nGanaste la ronda (+" << valorMano << " puntos)" << std::endl;
		puntosJugador += valorMano;
	}
	else {
		std::cout << "\nCPU gana la ronda (+" << valorMano << " puntos)" << std::endl;
		puntosCPU += valorMano;
	}

	int pj2 = puntosJugador;
	int pc2 = puntosCPU;

	int envidoJ = pj1 - pj0;
	int envidoC = pc1 - pc2;
	int trucoJ = pj2 - pj1;
	int trucoC = pc2 - pc1;

	std::cout << "\n=== RESUMEN DE LA RONDA ===\n";
	std::cout << "Envido: vos +" << envidoJ << " | CPU +" << envidoC<< "\n";
	std::cout << "Truco: vos +" << trucoJ << " | CPU +" << trucoC<< "\n";
	std::cout << "Total: vos +" << (pj2 - pj0) << " | CPU +" << (pc2 - pc0) << "\n";
	std::cout << "Marcador -> vos " << puntosJugador << " | CPU " << puntosCPU << "\n";
	std::cout << "==========================\n";
}

/// ==========================================================================================================================================

void jugarPartida(const Reglas& reglas) {
	int puntosJugador = 0;
	int puntosCPU = 0;

	bool jugadorEsMano = true;

	while (puntosJugador < reglas.puntosMax && puntosCPU < reglas.puntosMax) {

		jugarRonda(puntosJugador,puntosCPU,reglas,jugadorEsMano);

		jugadorEsMano = !jugadorEsMano;
	}
	if (puntosJugador == reglas.puntosMax) {
		cout << "\n*** GANASTE LA PARTIDA ***" << endl;
	} else{
	cout << "\n*** PERDISTE LA PARTIDA ***" << endl;
	}
}
