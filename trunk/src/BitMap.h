#ifndef BITMAP_H_
#define BITMAP_H_
using namespace std;
#include <string.h>
#include <sstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include "SDL/SDL.h"
#include "SDL/SDL_video.h"
#include "BitMap.h"

typedef struct {
  char bm[2];
  unsigned int tamano;//tamaño del archivo en total
  unsigned int reservado;
  unsigned int offset;//Donde empieza la información del bitmap
  unsigned int tamanoMetadatos;//tamaño del header del bitmap
  unsigned int altoEnPixels;
  unsigned int anchoEnPixels;
  short numeroPlanos;//no es importante para nosotros
  short profundidadColor;//debe ser 24
  unsigned int tipoCompresion;//debe ser 0
  unsigned int tamanoEstructura;//tamano de lo que seria la imagen
  unsigned int pixelPorMetroHorizontal;//no es importante para nosotros
  unsigned int pixelPorMetroVertical;//no es importante para nosotros
  unsigned int coloresUsados;//no es importante para nosotros
  unsigned int coloresImportantes;//no es importante para nosotros
} informacionDelBMP;

class BitMap {

private:
	informacionDelBMP* informacionImagen;
	bool seLevantoCorrectamente;
	SDL_Color** matrizDePixeles;
	unsigned long int bytesEnAncho,bytesEnAlto;

public:
	BitMap(string path);
	SDL_Color** getMatrizDeImagen();
	bool resizeTo(int alto, int ancho);
	bool seLevantoBienElArchivo();
	bool esUnaImagenCorrecta();//Se refiere a: es un bmp, si es de profundidad 24 bits y si no tiene ningun tipo de compresion
	unsigned int getAncho();//lo devuelve en pixels
	unsigned int getAlto();//lo devuelve en pixels
	virtual ~BitMap();
};

#endif /* BITMAP_H_ */
