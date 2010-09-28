#include "Pantalla.h"

Pantalla::Pantalla() {
	SDL_Init(SDL_INIT_VIDEO);
	pantalla=SDL_SetVideoMode(800,600,24,SDL_SWSURFACE);
	//Cuando termina el programa se cierra el SDL
	atexit(SDL_Quit);
	if (pantalla==0 or !(comprobarPantalla())) {
		cerr << "Error al crear ventana" << endl;
		cerr << SDL_GetError() << endl;
		exit(1);
	}
}

bool Pantalla::comprobarPantalla(){
	// Bytes que ocupa un pixel
	if(pantalla->format->BytesPerPixel!=3){
		return false;
	}
	return true;
}

void Pantalla::dibujarPixel(int x, int y, Pixel* pixel){
	//Si hay que bloquear la pantalla la bloquea, al final de la función la desbloquea
	if ( SDL_MUSTLOCK(pantalla) ){
		if ( SDL_LockSurface(pantalla) < 0 ){
			return;
		}
	}

	// p es un puntero al pixel que vamos a pintar
	//pitch es la cantidad de bytes que ocupa una linea de la pantalla
	Uint8 *p = (Uint8*)pantalla->pixels + (y*pantalla->pitch) + (x*(pantalla->format->BytesPerPixel));

	if (SDL_BYTEORDER==SDL_BIG_ENDIAN) {
		p[0]=(int)pixel->getRed();
		p[1]=(int)pixel->getGreen();
		p[2]=(int)pixel->getBlue();
	} else {
		p[0]=(int)pixel->getBlue();
		p[1]=(int)pixel->getGreen();
		p[2]=(int)pixel->getRed();
	}
	if ( SDL_MUSTLOCK(pantalla) ){
		SDL_UnlockSurface(pantalla);
	}
}

void Pantalla::actualizarPantalla(int x, int y, int w, int h){
	SDL_UpdateRect(pantalla, x, y, w, h);
}

Pantalla::~Pantalla() {
	SDL_Quit();
}
