#include "Jugador.h"

Jugador::Jugador(string path,string nombre, string plata, int id) {
	this->nombre=nombre;
	this->plata="$"+plata;
	this->path=path;
	this->id = id;
}

string Jugador::getNombre(){
	return this->nombre;
}
int Jugador::getId(){
	return this->id;
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
