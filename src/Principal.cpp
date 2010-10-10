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
	list<string>* jugadores=juego->pedirJugadores();
	list<string> * jugadoresAux=new list<string>(*jugadores);
	juego->pedirPoso();
	list<string>::iterator it=jugadoresAux->begin();
	int x=100;
	int y=100;
	while(it!=jugadoresAux->end()){
		path=juego->pedirImagenJugador(*it);
		juego->dibujarJugador(x,y,path);
		it++;
		it++;
		x+=100;
		y+=100;
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
