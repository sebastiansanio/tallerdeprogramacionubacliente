
#include "JugadorVirtual.h"

JugadorVirtual::JugadorVirtual(){

}
JugadorVirtual::JugadorVirtual(Jugador* jugador) {
// TODO Auto-generated constructor stub
	this->jugadorAsignado = jugador;
}

list<string>* JugadorVirtual::decidirJugada(list<Carta>* cartasJugador, list<Carta>* cartasMesa, int plata, int apuestaMax){
	list<Carta>::iterator it1;
	list<Carta>::iterator it2;
//	list<Carta>* cartasJugador = this->jugadorAsignado->getCartas();
	if(cartasJugador == NULL){
		this->instancia = 0;
		return NULL;
	}
	srand48(time(NULL));
	list<string>* decision = new list<string> ();
	it1 = cartasJugador->begin();
	Carta carta1 = *it1;
	it1++;
	Carta carta2 = *it1;
	int instanciaActual;
	if (cartasMesa->empty()) {
		instanciaActual = 1;
		if (!this->instancia == instanciaActual) {
			this->instancia = instanciaActual;
			if ((atoi(carta1.getNumero().c_str()) - atoi(
					carta2.getNumero().c_str()) > 7) and carta1.getNumero()
					!= "1" and carta2.getNumero() != "1"
					and (!(carta1.getPalo() == carta2.getPalo()))) {
				double numero = drand48();
				if (numero < 0.2) {
					string id = "Y";
					decision->push_front(id);
					return decision;
				} else {
					string id = "D";
					decision->push_front(id);
					decision->push_back("Poso");
					decision->push_back("0");
					return decision;
				}
			} else {
				if (apuestaMax == 0) {
					string id = "F";
					decision->push_front(id);
					return decision;
				} else {
					string id = "Y";
					decision->push_front(id);
					return decision;
				}
			}
		} else {
			if (apuestaMax == 0) {
				string id = "F";
				decision->push_front(id);
				return decision;
			} else if (apuestaMax - plata < plata / 2) {
				string id = "Y";
				decision->push_front(id);
				return decision;
			} else {
				string id = "D";
				decision->push_front(id);
				decision->push_back("Poso");
				decision->push_back("0");
				return decision;
			}
		}
	}
	it2 = cartasMesa->begin();
	if (cartasMesa->size() == 3) {
		instanciaActual = 2;
		if (!this->instancia == instanciaActual) {
			this->instancia = instanciaActual;
			if (true) {
				double numero = drand48();
				if (numero < 0.6) {
					string id = "Y";
					decision->push_front(id);
					return decision;
				} else {
					string id = "D";
					decision->push_front(id);
					decision->push_back("Poso");
					decision->push_back("0");
					return decision;
				}
			}
		} else {
			if (apuestaMax - plata < plata) {
				string id = "Y";
				decision->push_front(id);
				return decision;
			} else {
				string id = "D";
				decision->push_front(id);
				decision->push_back("Poso");
				decision->push_back("0");
				return decision;
			}
		}
	}
	if (cartasMesa->size() == 4) {
		instanciaActual = 3;
		if (!this->instancia == instanciaActual) {
			this->instancia = instanciaActual;
			if (true) {
				double numero = drand48();
				if (numero < 0.7) {
					string id = "Y";
					decision->push_front(id);
					return decision;
				} else {
					string id = "D";
					decision->push_front(id);
					decision->push_back("Poso");
					decision->push_back("0");
					return decision;
				}
			}
		} else {
			if (apuestaMax - plata < plata) {
				string id = "Y";
				decision->push_front(id);
				return decision;
			} else {
				string id = "D";
				decision->push_front(id);
				decision->push_back("Poso");
				decision->push_back("0");
				return decision;
			}
		}
	}
	if (cartasMesa->size() == 5) {
		instanciaActual = 4;
		if (!this->instancia == instanciaActual) {
			this->instancia = instanciaActual;
			if (true) {
				double numero = drand48();
				if (numero < 0.9) {
					string id = "Y";
					decision->push_front(id);
					return decision;
				} else {
					string id = "D";
					decision->push_front(id);
					decision->push_back("Poso");
					decision->push_back("0");
					return decision;
				}
			}
		} else {
			if (apuestaMax - plata < plata) {
				string id = "Y";
				decision->push_front(id);
				return decision;
			} else {
				string id = "D";
				decision->push_front(id);
				decision->push_back("Poso");
				decision->push_back("0");
				return decision;
			}
		}
	}
	return NULL;
}
JugadorVirtual::~JugadorVirtual() {
	// TODO Auto-generated destructor stub
}
