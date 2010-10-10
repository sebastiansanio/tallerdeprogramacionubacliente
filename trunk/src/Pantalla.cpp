#include "Pantalla.h"

Pantalla::Pantalla(int alto, int ancho) {
	SDL_Init(SDL_INIT_VIDEO);
	pantalla=SDL_SetVideoMode(ancho,alto,24,SDL_SWSURFACE);
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

void Pantalla::dibujarPixel(int x, int y, SDL_Color* color){
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
		p[0]=color->r;
		p[1]=color->g;
		p[2]=color->b;
	} else {
		p[0]=color->b;
		p[1]=color->g;
		p[2]=color->r;
	}
	if ( SDL_MUSTLOCK(pantalla) ){
		SDL_UnlockSurface(pantalla);
	}
}

void Pantalla::actualizarPantalla(int x, int y, int w, int h){
	SDL_UpdateRect(pantalla, x, y, w, h);
}

void Pantalla::dibujarBitMapDesdePos(BitMap bitmap,int x, int y){
	SDL_Color** matrizDelBitmap=bitmap.getMatrizDeImagen();
	for(unsigned int i=0;i<bitmap.getAlto();i++){
		for(unsigned int j=0;j<bitmap.getAncho();j++){
			this->dibujarPixel(j + x,i + y,&matrizDelBitmap[i][j]);
		}
	}
}
Pantalla::~Pantalla() {
}
