
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
	this->cartasJugador = cartasJugador;
	if(cartasJugador == NULL){
		this->instancia = 0;
		return NULL;
	}
	srand48(time(NULL));
	list<string>* decision = new list<string> ();
	Carta carta1 = cartasJugador->front();
	Carta carta2 = cartasJugador->back();
	string id;
	ostringstream sstream;
	int instanciaActual;
	if (cartasMesa->empty()) {
		instanciaActual = 1;
		if (this->instancia != instanciaActual) {
			this->instancia = instanciaActual;
			if ((atoi(carta1.getNumero().c_str()) - atoi(
					carta2.getNumero().c_str()) > 5) and carta1.getNumero()
					!= "1" and carta2.getNumero() != "1"
					and (!(carta1.getPalo() == carta2.getPalo()))) {
				double numero = drand48();
				if (numero < 0.2) {
					if(apuestaMax != 0)
						id = "Y";
					else
						id = "F";
					decision->push_front(id);
					return decision;
				} else {
					id = "D";
					decision->push_front(id);
					decision->push_back("Poso");
					decision->push_back("0");
					return decision;
				}
			} else {
				if (apuestaMax == 0) {
					id = "F";
					decision->push_front(id);
					return decision;
				} else {
					id = "Y";
					decision->push_front(id);
					return decision;
				}
			}
		} else {
			if (apuestaMax == 0) {
				id = "F";
				decision->push_front(id);
				return decision;
			} else if (apuestaMax  < plata / 2.5) {
				id = "Y";
				decision->push_front(id);
				return decision;
			} else {
				id = "D";
				decision->push_front(id);
				decision->push_back("Poso");
				decision->push_back("0");
				return decision;
			}
		}
	}
	it2 = cartasMesa->begin();
	//FLOP
	if (cartasMesa->size() == 3) {
		instanciaActual = 2;
		if (this->instancia != instanciaActual) {
			this->instancia = instanciaActual;
			double calidad = this->calidadMano(cartasMesa);
			if (calidad < 0.5) {
				double numero = drand48();
				if (numero < 0.2) {
					if (apuestaMax != 0)
						id = "Y";
					else
						id = "F";
					decision->push_front(id);
					return decision;
				} else {
					id = "D";
					int apuesto;
					if (apuestaMax < plata)
						apuesto = apuestaMax + ((plata-apuestaMax) / 18);
					else if(numero < 0.8)
						//ALL IN
						apuesto = plata;
					else
						apuesto = 0;
					ostringstream sstream;
					sstream << apuesto;
					decision->push_front(id);
					decision->push_back("Poso");
					decision->push_back(sstream.str());
					return decision;
				}
			} else if(calidad<0.75){
				double numero = drand48();
				id = "D";
				int apuesto;
				if (apuestaMax < plata)
					apuesto = apuestaMax + ((plata-apuestaMax) / 12);
				else if(numero < 0.8)
					//ALL IN
					apuesto = plata;
				else
					apuesto = 0;
				ostringstream sstream;
				sstream << apuesto;
				decision->push_front(id);
				decision->push_back("Poso");
				decision->push_back(sstream.str());
				return decision;
			}else if (calidad == 1) {
				id = "D";
				int apuesto;
				if (apuestaMax < plata)
					apuesto =  apuestaMax + ((plata-apuestaMax) / 10);
				else {
					//Hago All-In
					apuesto = plata;
				}
				ostringstream sstream;
				sstream << apuesto;
				decision->push_front(id);
				decision->push_back("Poso");
				decision->push_back(sstream.str());
				return decision;
			} else {
				if(apuestaMax != 0)
					id = "Y";
				else
					id = "F";
				decision->push_front(id);
				return decision;
			}
		} else {
			if (apuestaMax < plata / 2 or plata==0) {
				if(apuestaMax != 0)
					id = "Y";
				else
					id = "F";
				decision->push_front(id);
				return decision;
			} else {
				id = "D";
				decision->push_front(id);
				decision->push_back("Poso");
				decision->push_back("0");
				return decision;
			}
		}
	}
	//TURN
	if (cartasMesa->size() == 4) {
		instanciaActual = 3;
		if (!this->instancia == instanciaActual) {
			this->instancia = instanciaActual;
			double calidad = this->calidadMano(cartasMesa);
			if (calidad < 0.4) {
				double numero = drand48();
				if (numero < 0.8) {
					if (apuestaMax != 0)
						id = "Y";
					else
						id = "F";
					decision->push_front(id);
					return decision;
				} else {
					id = "D";
					int apuesto;
					if (apuestaMax < plata)
						apuesto = apuestaMax + ((plata - apuestaMax) / 10);
					else {
						double numero = drand48();
						//Hago All-In con cierta probabilidad
						if (numero > 0.4)
							apuesto = plata;
						else
							apuesto = 0;
					}
					ostringstream sstream;
					sstream << apuesto;
					decision->push_front(id);
					decision->push_back("Poso");
					decision->push_back(sstream.str());
					return decision;
				}
			}else if(calidad<1){
				double numero = drand48();
				id = "D";
				int apuesto;
				if (apuestaMax < plata)
					apuesto = apuestaMax + ((plata-apuestaMax) / 12);
				else if(numero < 0.8)
					//ALL IN
					apuesto = plata;
				else
					apuesto = 0;
				ostringstream sstream;
				sstream << apuesto;
				decision->push_front(id);
				decision->push_back("Poso");
				decision->push_back(sstream.str());
				return decision;
			} else if (calidad == 1) {
				id = "D";
				int apuesto;
				if (apuestaMax < plata)
					apuesto = apuestaMax + ((plata - apuestaMax) / 8);
				else {
					//Hago All-In
					apuesto = plata;
				}
				ostringstream sstream;
				sstream << apuesto;
				decision->push_front(id);
				decision->push_back("Poso");
				decision->push_back(sstream.str());
				return decision;
			} else {
				id = "Y";
				decision->push_front(id);
				return decision;

			}
		} else {
			if (apuestaMax < plata or plata==0) {
				if (apuestaMax != 0)
					id = "Y";
				else
					id = "F";
				decision->push_front(id);
				return decision;
			} else {
				id = "D";
				decision->push_front(id);
				decision->push_back("Poso");
				decision->push_back("0");
				return decision;
			}
		}
	}
	//RIVER
	if (cartasMesa->size() == 5) {
		instanciaActual = 4;
		if (!this->instancia == instanciaActual) {
			this->instancia = instanciaActual;
			double calidad = this->calidadMano(cartasMesa);
			if (calidad < 0.3) {
				double numero = drand48();
				if (numero < 0.8) {
					if (apuestaMax != 0)
						id = "Y";
					else
						id = "F";
					decision->push_front(id);
					return decision;
				} else {
					int apuesto;
					if (apuestaMax < plata)
						apuesto = apuestaMax + ((plata - apuestaMax) / 10);
					else {
						double numero = drand48();
						//Hago All-In con cierta probabilidad
						if (numero > 0.3)
							apuesto = plata;
						else
							apuesto = 0;
					}
					ostringstream sstream;
					sstream << apuesto;
					decision->push_front(id);
					decision->push_back("Poso");
					decision->push_back(sstream.str());
					return decision;
				}
			} else if (calidad == 1) {
				id = "D";
				int apuesto;
				if (apuestaMax < plata)
					apuesto = apuestaMax + ((plata - apuestaMax) / 4);
				else {
					//Hago All-In
					apuesto = plata;
				}
				ostringstream sstream;
				sstream << apuesto;
				decision->push_front(id);
				decision->push_back("Poso");
				decision->push_back(sstream.str());
				return decision;
			} else {
				if (apuestaMax != 0)
					id = "Y";
				else
					id = "F";
				decision->push_front(id);
				return decision;

			}
		} else {
			if (apuestaMax < plata or plata==0) {
				if (apuestaMax != 0)
					id = "Y";
				else
					id = "F";
				decision->push_front(id);
				return decision;
			} else {
				id = "D";
				decision->push_front(id);
				decision->push_back("Poso");
				decision->push_back("0");
				return decision;
			}
		}
	}
	return NULL;
}

double JugadorVirtual::calidadMano(list<Carta>* cartasMesa){
	int numero1 = atoi(this->cartasJugador->front().getNumero().c_str());
	int numero2 = atoi(this->cartasJugador->back().getNumero().c_str());
	string palo1 = this->cartasJugador->front().getPalo();
	string palo2 = this->cartasJugador->back().getPalo();
	if (this->instancia == 2) {
		list<Carta>::iterator it=cartasMesa->begin();
		int numeroCarta1=atoi(it->getNumero().c_str());
		it++;
		int numeroCarta2=atoi(it->getNumero().c_str());
		it++;
		int numeroCarta3=atoi(it->getNumero().c_str());
		if (numero1 == numero2 or numero1==numeroCarta1 or numero1==numeroCarta2 or numero1==numeroCarta3 or numero2==numeroCarta1 or numero2==numeroCarta2 or numero2==numeroCarta3) {
			if(numero1>7 or numero2>7 or numero1==1 or numero2==1){
				return 0.22 * 3;
			}else{
				return 0.11 * 3;
			}
		} else if (palo1 == palo2) {
			return 0.11 * 3.5;
		} else if (numero1 - numero2 == 1 or numero1 - numero2 == -1) {
			return 0.1 * 3;
		} else {
			return 0.3 * 0.75;
		}
	}
	else if(this->instancia == 3){
		list<Carta>::iterator it;
		it = cartasMesa->begin();
		double acumulado = 0;
		for(unsigned int i = 0 ; i < cartasMesa->size() ; i++){
			if(numero1 == atoi(it->getNumero().c_str())){
				if(numero1>9){
					acumulado += 0.6;
				}else{
					acumulado+=0.3;
				}
			}else if(numero2 == atoi(it->getNumero().c_str())){
				if(numero2>9){
					acumulado += 0.6;
				}else{
					acumulado+=0.3;
				}
			}else if(palo1 == it->getPalo() or palo2 == it->getPalo())
				acumulado += 0.3;
			else
				acumulado += 0.15;
			it++;
		}
		if(acumulado >= 1)
			return 1;
		else
			return acumulado;
	}
	else if(this->instancia == 4){
		list<Carta>::iterator it;
		it = cartasMesa->begin();
		double acumulado = 0;
		for (unsigned int i = 0; i < cartasMesa->size(); i++) {
			if(this->tieneEscalera(cartasMesa)){
				return 1;
			}else if(numero1 == atoi(it->getNumero().c_str())){
				if(numero1>9){
					acumulado += 0.3;
				}else{
					acumulado+=0.2;
				}
			}else if(numero2 == atoi(it->getNumero().c_str())){
				if(numero2>9){
					acumulado += 0.3;
				}else{
					acumulado+=0.2;
				}
			}else if (palo1 == it->getPalo() or palo2 == it->getPalo())
				acumulado += 0.3;
			else
				acumulado += 0.15;
			it++;
		}
		if (acumulado >= 1)
			return 1;
		else
			return acumulado;
	}
	else {
		return 0.5 ;
	}
}

bool JugadorVirtual::tieneEscalera(list<Carta>* cartasMesa){
	int numero1=atoi(this->cartasJugador->front().getNumero().c_str());
	int numero2=atoi(this->cartasJugador->back().getNumero().c_str());
	list<Carta>::iterator it;
	it = cartasMesa->begin();
	int * cartasNumero=new int[cartasMesa->size() + 2];
	cartasNumero[0]=numero1;
	cartasNumero[1]=numero2;
	for (unsigned int i = 2; i < cartasMesa->size() + 2; i++) {
		cartasNumero[i]=atoi(it->getNumero().c_str());
		it++;
	}
	for(int i=0;i<7;i++){
		for(int j=i;i<7;i++){
			if(cartasNumero[i]>cartasNumero[j]){
				int aux=cartasNumero[i];
				cartasNumero[i]=cartasNumero[j];
				cartasNumero[j]=aux;
			}
		}
	}

	if((cartasNumero[0]+1)==(cartasNumero[1])){
		if((cartasNumero[1]+1)==(cartasNumero[2])){
			if((cartasNumero[2]+1)==(cartasNumero[3])){
				if((cartasNumero[3]+1)==(cartasNumero[4])){
					return true;
				}else{
					return false;
				}
			}else{
				return false;
			}
		}else{
			if((cartasNumero[2]+1)==(cartasNumero[3])){
				if((cartasNumero[3]+1)==(cartasNumero[4])){
					if((cartasNumero[4]+1)==(cartasNumero[5])){
						if((cartasNumero[5]+1)==(cartasNumero[6])){
							return true;
						}else{
							return false;
						}
					}else{
						return false;
					}
				}else{
					return false;
				}
			}else{
				return false;
			}
		}
	}else{
		if((cartasNumero[1]+1)==(cartasNumero[2])){
					if((cartasNumero[2]+1)==(cartasNumero[3])){
						if((cartasNumero[3]+1)==(cartasNumero[4])){
							return true;
						}else{
							return false;
						}
					}else{
						return false;
					}
				}else{
					if((cartasNumero[2]+1)==(cartasNumero[3])){
						if((cartasNumero[3]+1)==(cartasNumero[4])){
							if((cartasNumero[4]+1)==(cartasNumero[5])){
								if((cartasNumero[5]+1)==(cartasNumero[6])){
									return true;
								}else{
									return false;
								}
							}else{
								return false;
							}
						}else{
							return false;
						}
					}else{
						return false;
					}
				}
	}
}
JugadorVirtual::~JugadorVirtual() {
	// TODO Auto-generated destructor stub
}
