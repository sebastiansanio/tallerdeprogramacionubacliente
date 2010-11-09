#ifndef JUGADOR_H_
#define JUGADOR_H_
using namespace std;
#include <string>
#include <string.h>
#include "Carta.h"
#include <list>

class Jugador {
private:
	string path;
	string nombre;
	string plata;
	int id;//Para saber la posicion relativa en la pantalla
	list<Carta> * cartas;
public:
	Jugador(string path, string nombre, string plata, int id);
	list<Carta> * getCartas();
	void setCartas(list<Carta> * cartasNuevas);
	string getNombre();
	string getPlata();
	string getPath();
	bool modificarPlataEn(int plata);
	int getId();
	void setPath(string path);
	virtual ~Jugador();
};

#endif /* JUGADOR_H_ */
