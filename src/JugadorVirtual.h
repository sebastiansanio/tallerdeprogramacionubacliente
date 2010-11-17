
#ifndef JUGADORVIRTUAL_H_
#define JUGADORVIRTUAL_H_

#include "Carta.h"
#include "Jugador.h"
#include <string>
#include <stdlib.h>

class JugadorVirtual {
public:
	Jugador* jugadorAsignado;
	int instancia; //Numero de instancia en la mano, existen 5
	JugadorVirtual();
	JugadorVirtual(Jugador* jugador);
	list<string>* decidirJugada(list<Carta>* cartasJugador, list<Carta>* cartasMesa, int plata, int apuestaMax);
	virtual ~JugadorVirtual();
};

#endif /* JUGADORVIRTUAL_H_ */
