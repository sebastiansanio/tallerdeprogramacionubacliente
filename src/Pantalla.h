/*
 * Pantalla.h
 *
 *  Created on: 26/09/2010
 *      Author: damian
 */

#ifndef PANTALLA_H_
#define PANTALLA_H_

#include <iostream>
#include <fstream>
#include "SDL/SDL.h"
#include "SDL/SDL_video.h"
#include <sstream>
#include "BitMap.h"
using namespace std;

class Pantalla {
private:
	SDL_Surface *pantalla;
public:
	Pantalla(int alto, int ancho);
	bool comprobarPantalla();
	void dibujarPixel(int x, int y,SDL_Color* color);
	void actualizarPantalla(int x, int y, int w, int h);
	void dibujarBitMap(BitMap bitmap);
	virtual ~Pantalla();
};

#endif /* PANTALLA_H_ */
