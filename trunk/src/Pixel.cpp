/*
 * Pixel.cpp
 *
 *  Created on: 26/09/2010
 *      Author: damian
 */

#include "Pixel.h"

Pixel::Pixel(char red,char green,char blue) {
	this->red=red;
	this->green=green;
	this->blue=blue;
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

Pixel::~Pixel() {
}
