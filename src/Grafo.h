#ifndef GRAFO_H_
#define GRAFO_H_

#include "Nodo.h"

class Grafo {
private:
	list<Nodo*>* nodos;
public:
	Grafo();
	void agregarNodo(Nodo* unNodo);
	Nodo* getNodoPorClave(int clave);
	virtual ~Grafo();
};

#endif /* GRAFO_H_ */
