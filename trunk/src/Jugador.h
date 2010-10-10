/*
 * Jugador.h
 *
 *  Created on: 10/10/2010
 *      Author: gaston
 */

#ifndef JUGADOR_H_
#define JUGADOR_H_
using namespace std;
#include <string>
#include <string.h>

class Jugador {
private:
	string path;
	string nombre;
	string plata;
public:
	Jugador(string path, string nombre, string plata);
	string getNombre();
	string getPlata();
	string getPath();
	void setPath(string path);
	virtual ~Jugador();
};

#endif /* JUGADOR_H_ */
