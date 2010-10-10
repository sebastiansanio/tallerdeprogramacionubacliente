#ifndef JUEGO_H_
#define JUEGO_H_

#define PATHARCHIVOCONF "config.ini"
#define PATHESCENARIO "pantalla.bmp"
#define ANCHO 1280
#define ALTO 768
#include "Pantalla.h"
#include "Cliente.h"
#include "ParserCliente.h"
#include "ParserResultadoCliente.h"

class Juego {
private:
	Pantalla* pantalla;
	Cliente* cliente;
	ParserCliente* parser;
	ParserResultadoCliente* parserResultado;
	list<string> * jugadores;//Nombre de los jugadores conectados para que los dibuje
	list<string> * cartas;//Las que salieron, valor y palo
	string posoAcumulado;//Poso que esta acumulado
public:
	Juego();
	void dibujarEscenario();
	void pedirEscenario();
	void pedirJugadores();
	void pedirCartas();
	void pedirPoso();
	virtual ~Juego();
};

#endif /* JUEGO_H_ */