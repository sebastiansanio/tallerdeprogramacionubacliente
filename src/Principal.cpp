using namespace std;
#include <string>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "Juego.h"
#include "math.h"

int main(){
	Juego* juego = new Juego();
	string path;
	path = juego->pedirEscenario();
	juego->dibujarEscenario(path);
	list<Carta>* cartas = juego->pedirCartas();
	list<Jugador>* jugadores = juego->pedirJugadores();
	juego->pedirPoso();
	list<Jugador>::iterator it = jugadores->begin();
	while (it != jugadores->end()) {
		path = juego->pedirImagenJugador(&(*it));
		juego->dibujarJugador(*it);
		it++;
	}
	list<Carta>::iterator it2 = cartas->begin();
	while (it2 != cartas->end()) {
		juego->dibujarCarta(*it2);
		it2++;
	}
	juego->actualizarPantalla();

	//Empiezo a mostrar
	SDL_Event evento;
	bool terminar=false;
	while(!terminar){
		while(SDL_PollEvent(&evento)) {
			if((evento.type == SDL_QUIT)){
				terminar=true;
			}
		}
	}
	delete juego;
	return 0;
}
