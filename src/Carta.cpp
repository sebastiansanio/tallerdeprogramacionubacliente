#include "Carta.h"

Carta::Carta(string path, string palo, string numero, int id) {
	this->palo=palo;
	this->numero=numero;
	this->path=path;
	this->id = id;
}

string Carta::getPalo(){
	return this->palo;
}
int Carta::getId(){
	return this->id;
}

string Carta::getPath(){
	return this->path;
}

string Carta::getNumero(){
	return this->numero;
}

void Carta::setPath(string path){
	this->path=path;
}

Carta::~Carta() {
}
