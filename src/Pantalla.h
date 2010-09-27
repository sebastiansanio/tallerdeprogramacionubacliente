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
#include "Pixel.h"
#include "SDL/SDL.h"
#include "SDL/SDL_video.h"
using namespace std;

class Pantalla {
private:
	SDL_Surface *pantalla;
public:
	Pantalla();
	bool comprobarPantalla();
	void dibujarPixel(int x, int y,Pixel* pixel);
	void actualizarPantalla(int x, int y, int w, int h);
	virtual ~Pantalla();
};

#endif /* PANTALLA_H_ */
