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
	path=juego->pedirEscenario();
	juego->dibujarEscenario(path);
	juego->pedirCartas();
	list<Jugador>* jugadores=juego->pedirJugadores();
	juego->pedirPoso();
	list<Jugador>::iterator it=jugadores->begin();
	int x=220;
	int y=220;
	while(it!=jugadores->end()){
		path=juego->pedirImagenJugador(&(*it));
		juego->dibujarJugador(x,y,*it);
		it++;
		x+=100;
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
