#include "Grafo.h"

Grafo::Grafo() {
	this->nodos = new list<Nodo*>;
}

void Grafo::agregarNodo(Nodo* unNodo){
	this->nodos->push_back(unNodo);
}

Nodo* Grafo::getNodoPorClave(int clave){
	list<Nodo*>::iterator iterador=this->nodos->begin();
	while(iterador!=this->nodos->end()){
		if((*iterator)->getClave==clave){
			return *iterator;
		}
		iterator++;
	}
}

Grafo::~Grafo() {
	delete this->nodos;
}