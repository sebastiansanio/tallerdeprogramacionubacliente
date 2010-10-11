#ifndef JUEGO_H_
#define JUEGO_H_

#define PATHARCHIVOCONF "config.ini"
#define PATHESCENARIO "pantalla.bmp"
#define ANCHO 800
#define ALTO 600
#include "Pantalla.h"
#include "Cliente.h"
#include "ParserCliente.h"
#include "ParserResultadoCliente.h"
#include "Jugador.h"
#include "Carta.h"

class Juego {
private:
	Pantalla* pantalla;
	Cliente* cliente;
	ParserCliente* parser;
	ParserResultadoCliente* parserResultado;
	list<Jugador> * jugadores;//Nombre de los jugadores conectados para que los dibuje
	list<Carta> * cartas;//Las que salieron, valor y palo
	string posoAcumulado;//Poso que esta acumulado
	informacionConfiguracion* infoconfig;
public:
	Juego();
	void dibujarEscenario(string path);
	void dibujarJugador(Jugador jugadorADibujar);
	void dibujarCarta(Carta cartaADibujar);
	void dibujarBoton(string textoBoton, int pos);
	bool verificarResolucion(unsigned int alto,unsigned  int ancho);
	string pedirEscenario();
	string pedirImagenJugador(Jugador * jugador);
	list<Jugador>* pedirJugadores();
	list<Carta>* pedirCartas();
	void pedirCartasComunitarias();
	void pedirPoso();
	void actualizarPantalla();
	virtual ~Juego();
};

#endif /* JUEGO_H_ */
