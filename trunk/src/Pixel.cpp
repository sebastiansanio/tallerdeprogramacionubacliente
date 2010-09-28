#include "Pixel.h"

Pixel::Pixel(char red,char green,char blue,unsigned long int posX, unsigned long int posY) {
	this->red=red;
	this->green=green;
	this->blue=blue;
	this->posX=posX;
	this->posY=posY;
}

char Pixel::getRed(){
	return this->red;
}

char Pixel::getGreen(){
	return this->green;
}

char Pixel::getBlue(){
	return this->blue;
}

unsigned long int Pixel::getPosX(){

	return this->posX;
}

unsigned long int Pixel::getPosY(){
	return this->posY;
}

Pixel::~Pixel() {
}
