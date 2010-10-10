/*
 * Jugador.cpp
 *
 *  Created on: 10/10/2010
 *      Author: gaston
 */

#include "Jugador.h"

Jugador::Jugador(string path,string nombre, string plata) {
	this->nombre=nombre;
	this->plata=plata;
	this->path=path;
}

string Jugador::getNombre(){
	return this->nombre;
}

string Jugador::getPath(){
	return this->path;
}

string Jugador::getPlata(){
	return this->plata;
}

void Jugador::setPath(string path){
	this->path=path;
}

Jugador::~Jugador() {
	// TODO Auto-generated destructor stub
}
