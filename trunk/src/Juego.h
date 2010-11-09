#ifndef JUEGO_H_
#define JUEGO_H_

#define PATHARCHIVOCONF "config.ini"
#define PATHESCENARIO "pantalla.bmp"
#include "Pantalla.h"
#include "Cliente.h"
#include "ParserCliente.h"
#include "ParserResultadoCliente.h"
#include "Jugador.h"
#include "Carta.h"

typedef enum{
	NINGUNO,
	USUARIO,
	CONTRASENA,
	ARCHIVO,
} CasilleroTexto;

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
	string nombreJugador;
public:
	Juego();
	void empezarPartida();//Manda una F para que sepa que se empieza la partida
	void dibujarPantallaPrincipal();
	void dibujarPantallaLogin(bool usuarioIncorrecto, int cantidadIntentos,bool jugador_observador);
	void dibujarPantallaRegistro(int cantidadIntentos);
	void dibujarPantallaObservacion();
	void dibujarEscenario(string path);
	void dibujarJugador(Jugador jugadorADibujar);
	void dibujarCarta(Carta cartaADibujar);
	void dibujarBoton(string textoBoton, int pos);
	void dibujarPoso();
	void dibujarCartaJugador(Jugador * jugador);
	bool esMiTurno();
	bool verificarResolucion(unsigned int alto,unsigned  int ancho);
	string pedirEscenario();
	string pedirImagenJugador(Jugador * jugador);
	bool enviarImagenJugador(string ruta,string jugador);
	list<Jugador>* pedirJugadores();
	list<Carta>* pedirCartas();
	list<Carta>* pedirCartasJugador(Jugador * jugador);
	void pedirCartasComunitarias();
	void pedirPoso();
	void actualizarPantalla();
	void informarError(string idOperacion, string tipoError, string mensaje);
	bool validarJugador(string usuario, string pass);
	bool registrarJugador(string usuario, string pass);
	void jugar(bool jugador_observador, bool jugador_virtual);
	Jugador * getJugador(int id);
	virtual ~Juego();
};

#endif /* JUEGO_H_ */
