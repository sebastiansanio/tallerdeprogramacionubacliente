#include "Jugador.h"

Jugador::Jugador(string path,string nombre, string plata, int id) {
	this->nombre=nombre;
	this->plata=plata;
	this->path=path;
	this->id = id;
	this->cartas=NULL;
	this->imagenPedida=false;
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

list<Carta>* Jugador::getCartas(){
	return this->cartas;
}

bool Jugador::imagenFuePedida(){
	return this->imagenPedida;
}

void Jugador::imagenEstablecida(){
	this->imagenPedida=true;
}
void Jugador::setCartas(list<Carta> * cartasNuevas){
	if(this->cartas!=NULL) delete this->cartas;
	this->cartas=cartasNuevas;
}

Jugador::~Jugador() {
	// TODO Auto-generated destructor stub
}
