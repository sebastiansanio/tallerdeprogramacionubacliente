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
	juego->pedirJugadores();
	juego->pedirPoso();
	path=juego->pedirImagenJugador("pepe");
	juego->dibujarJugador(350,210,path);
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
