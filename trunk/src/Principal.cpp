using namespace std;
#include <string>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "Juego.h"
#include "math.h"
#include "Jugador.h"
#include "Carta.h"

int main(){
	bool usuarioValido=false;
		//empieza la interacción usuario - cliente - servidor
	Juego juego;
	juego.dibujarPantallaPrincipal();
	SDL_Event evento;
	bool terminar=false;
	while(!terminar){
		while(SDL_PollEvent(&evento)) {
			if((evento.type == SDL_QUIT)){
				terminar=true;
			}
		}
	}
	return 0;
}
