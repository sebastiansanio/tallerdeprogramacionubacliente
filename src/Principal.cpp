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
	Juego* juego = new Juego();

	while(!usuarioValido){

		system("clear");
		cout<<"Ingrese usuario: ";
		string user;
		string pass;

		cin>>user;
		cout<<"Ingrese password: ";
		cin>>pass;

		if((user!="" )&&(pass!="")){
			usuarioValido=juego->validarJugador(user, pass);
		}
		 if (!usuarioValido){
			 exit(0);
		 }

	}
	string path,pathEscenario;
	pathEscenario = juego->pedirEscenario();
	list<Carta>* cartas = juego->pedirCartas();
	list<Jugador>* jugadores = juego->pedirJugadores();
	juego->pedirPoso();
	juego->dibujarEscenario(pathEscenario);
	list<Jugador>::iterator it = jugadores->begin();
	while (it != jugadores->end()) {
		path = juego->pedirImagenJugador(&(*it));
		juego->dibujarJugador(*it);
		it++;
	}
	if(jugadores->size()<6){
		for(int i=(jugadores->size() + 1);i<7;i++){
			Jugador jugador("ImagenVacio.bmp"," "," ",i);
			juego->dibujarJugador(jugador);
		}
	}
	list<Carta>::iterator it2 = cartas->begin();
	while (it2 != cartas->end()) {
		juego->dibujarCarta(*it2);
		it2++;
	}
	if(cartas->size()<5){
		for(int i=(cartas->size()+1);i<6;i++){
			Carta carta("Imagen-Carta.bmp","Imagen","Carta",i);
			juego->dibujarCarta(carta);
		}
	}
	//Dibujamos los botones
	juego->dibujarBoton("No Ir",1);
	juego->dibujarBoton("Igualar",2);
	juego->dibujarBoton("Subir el doble",3);
	juego->dibujarPoso();
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
