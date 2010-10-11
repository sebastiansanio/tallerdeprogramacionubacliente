#include "Juego.h"

Juego::Juego() {
	this->parser=new ParserCliente();
	this->parserResultado=new ParserResultadoCliente();
	ParserCliente *parserAux=new ParserCliente(PATHARCHIVOCONF);

	if(parserAux->comprobarSintaxis()){

		this->infoconfig = parserAux->getInformacionConfig();
		cout << "alto y ancho" << endl;
		cout << this->infoconfig->alto << endl;
		cout << this->infoconfig->ancho << endl;

		if(!this->verificarResolucion(this->infoconfig->alto,this->infoconfig->ancho)){
			list<string>* mensajesError = new list<string>();
			mensajesError->push_back("E");
			string mensaje = "Resolucion invalida en el archivo config.ini";
			mensajesError->push_back(mensaje);
			parserAux->registrarError("G", mensajesError);
			cout << mensaje << endl;
			sleep(4);
			exit(0);
		}

		this->pantalla = new Pantalla(this->infoconfig->alto, this->infoconfig->ancho);
		cout << "puerto e ip" << endl;
		cout << this->infoconfig->puerto << endl;
		cout << this->infoconfig->ip << endl;
		this->cliente=new Cliente(this->infoconfig->puerto, this->infoconfig->ip);
		//this->cliente = new Cliente();
		cliente->conectar();
	}
	else {
		cout << "Sintaxis incorrecta" << endl;
	}
	delete parserAux;
}

bool Juego::verificarResolucion(unsigned int alto,unsigned  int ancho){
	if((alto==480&&ancho==640) || (alto==600 && ancho==800) || (alto==768 && ancho==1024) || (alto==768 && ancho==1280))
		return true;
	else
		return false;

}

Juego::~Juego() {
	delete pantalla;
	delete cliente;
	delete parser;
	delete parserResultado;
	delete infoconfig;
}

string Juego::pedirEscenario(){
	string ruta=PATHESCENARIO;
	//Pido el escenario
	string idOperacion="E";
	list<string>* operandos=new list<string>();
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	delete operandos;
	cliente->enviar(xml);
	cliente->recibirArchivo(ruta);
	return PATHESCENARIO;
}

string Juego::pedirImagenJugador(Jugador * jugador){
	string ruta=jugador->getNombre() + ".bmp";
	jugador->setPath(ruta);
	//Pido el escenario
	string idOperacion="I";
	list<string>* operandos=new list<string>();
	list<string>::iterator it=operandos->begin();
	it=operandos->insert(it,"jugador");
	it++;
	it=operandos->insert(it,jugador->getNombre());
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	delete operandos;
	cliente->enviar(xml);
	cliente->recibirArchivo(ruta);
	return ruta;
}

void Juego::dibujarEscenario(string path){
	BitMap* escenario=new BitMap(path);
	if(escenario->esUnaImagenCorrecta()){
		escenario->resizeTo(this->infoconfig->alto, this->infoconfig->ancho);
		pantalla->dibujarBitMapDesdePos((*escenario),0,0);
	}else{
		cout<<"No es una imagen corecta"<<endl;
	}
}


void Juego::dibujarJugador(Jugador jugadorADibujar){
	BitMap* jugador = new BitMap(jugadorADibujar.getPath());
	//dibujaria con el sdl_ttf el nombre y la plata al lado de la imagen
	if (jugador->esUnaImagenCorrecta()) {
		int tamImagen = this->infoconfig->ancho / 10;
		jugador->resizeTo(tamImagen, tamImagen);
		const char* nombre = jugadorADibujar.getNombre().c_str();
		const char* plata = jugadorADibujar.getPlata().c_str();
		int tamfuente = this->infoconfig->ancho / 56;
		int factor = this->infoconfig->alto / 110;
		if (jugadorADibujar.getId() == 1) {
			pantalla->dibujarBitMapDesdePosCircular(*jugador,
					this->infoconfig->ancho / 8, this->infoconfig->alto / 2);
			pantalla->escribirTextoDesdePos(nombre, (this->infoconfig->ancho / 8)+(tamImagen/4), (this->infoconfig->alto / 2)+tamImagen+factor, tamfuente );
			pantalla->escribirTextoDesdePos(plata, (this->infoconfig->ancho / 8)+(tamImagen/4), (this->infoconfig->alto / 2)+tamImagen+factor+tamfuente, tamfuente );
		} else if (jugadorADibujar.getId() == 2) {
			pantalla->dibujarBitMapDesdePosCircular(*jugador,
					this->infoconfig->ancho / 5.4, this->infoconfig->alto / 3.2);
			pantalla->escribirTextoDesdePos(nombre, (this->infoconfig->ancho / 5.4)+(tamImagen/4), (this->infoconfig->alto / 3.2)-(2*tamfuente)-factor, tamfuente );
			pantalla->escribirTextoDesdePos(plata, (this->infoconfig->ancho / 5.4)+(tamImagen/4), (this->infoconfig->alto / 3.2)-(tamfuente)-factor, tamfuente );
		} else if (jugadorADibujar.getId() == 3) {
			pantalla->dibujarBitMapDesdePosCircular(*jugador,
					this->infoconfig->ancho / 2.8, this->infoconfig->alto / 3.9);
			pantalla->escribirTextoDesdePos(nombre, (this->infoconfig->ancho / 2.8)+(tamImagen/4), (this->infoconfig->alto / 3.9)-(2*tamfuente)-factor, tamfuente );
			pantalla->escribirTextoDesdePos(plata, (this->infoconfig->ancho / 2.8)+(tamImagen/4), (this->infoconfig->alto / 3.9)-(tamfuente)-factor, tamfuente );
		} else if (jugadorADibujar.getId() == 4) {
			pantalla->dibujarBitMapDesdePosCircular(*jugador,
					this->infoconfig->ancho / 1.8, this->infoconfig->alto / 3.9);
			pantalla->escribirTextoDesdePos(nombre, (this->infoconfig->ancho / 1.8)+(tamImagen/4), (this->infoconfig->alto / 3.9)-(2*tamfuente)-factor, tamfuente );
			pantalla->escribirTextoDesdePos(plata, (this->infoconfig->ancho / 1.8)+(tamImagen/4), (this->infoconfig->alto / 3.9)-(tamfuente)-factor, tamfuente );
		} else if (jugadorADibujar.getId() == 5) {
			pantalla->dibujarBitMapDesdePosCircular(*jugador,
					this->infoconfig->ancho / 1.4, this->infoconfig->alto / 3.2);
			pantalla->escribirTextoDesdePos(nombre, (this->infoconfig->ancho / 1.4)+(tamImagen/4), (this->infoconfig->alto / 3.2)-(2*tamfuente)-factor, tamfuente );
			pantalla->escribirTextoDesdePos(plata, (this->infoconfig->ancho / 1.4)+(tamImagen/4), (this->infoconfig->alto / 3.2)-(tamfuente)-factor, tamfuente );
		} else if (jugadorADibujar.getId() == 6) {
			pantalla->dibujarBitMapDesdePosCircular(*jugador,
					this->infoconfig->ancho / 1.3, this->infoconfig->alto / 2);
			pantalla->escribirTextoDesdePos(nombre, (this->infoconfig->ancho / 1.3)+(tamImagen/4), (this->infoconfig->alto / 2)+tamImagen+factor, tamfuente );
			pantalla->escribirTextoDesdePos(plata, (this->infoconfig->ancho / 1.3)+(tamImagen/4), (this->infoconfig->alto / 2)+tamImagen+factor+tamfuente, tamfuente );
		}
		//pantalla->dibujarBitMapDesdePos((*jugador),x,y);
	} else {
		cout<<"No es una imagen corecta"<<endl;
	}
}


list<Jugador>* Juego::pedirJugadores(){
	string idOperacion="J";
	list<string>* operandos=new list<string>();
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	delete operandos;
	cliente->enviar(xml);
	char * respuesta=cliente->recibirRespuesta();
	list<string>* jugadoresAux=parserResultado->getJugadores(respuesta);
	list<string>::iterator it;
	list <Jugador> * jugadores=new list<Jugador>();
	list<Jugador>::iterator iterador=jugadores->begin();
	it = jugadoresAux->begin();
	for(unsigned int i = 0; i < jugadoresAux->size()/2; i++){
		string nombre=(*it);
		it++;
		string plata=(*it);
		it++;
		iterador=jugadores->insert(iterador,Jugador("",nombre,plata,i+1));
		iterador++;
	}
	this->jugadores = jugadores;
	return jugadores;
}

void Juego::dibujarCarta(Carta cartaADibujar){
	BitMap* carta = new BitMap(cartaADibujar.getPalo()+"-"+cartaADibujar.getNumero()+".bmp");
	//BitMap* carta = new BitMap("quilmes.bmp");
	if (carta->esUnaImagenCorrecta()) {
		int tamImagen = this->infoconfig->ancho / 22;
		carta->resizeTo(2 * tamImagen, tamImagen);
		int id = cartaADibujar.getId();
		if (id == 1) {
			pantalla->dibujarBitMapDesdePos(*carta,
					this->infoconfig->ancho / 3.15, (this->infoconfig->alto / 2.3));
		} else if (id == 2) {
			pantalla->dibujarBitMapDesdePos(*carta,
					this->infoconfig->ancho / 2.58, (this->infoconfig->alto / 2.3));
		} else if (id == 3) {
			pantalla->dibujarBitMapDesdePos(*carta,
					this->infoconfig->ancho / 2.2, (this->infoconfig->alto / 2.3));
		} else if (id == 4) {
			pantalla->dibujarBitMapDesdePos(*carta,
					this->infoconfig->ancho / 1.88, (this->infoconfig->alto / 2.3));
		} else if (id == 5) {
			pantalla->dibujarBitMapDesdePos(*carta,
					this->infoconfig->ancho / 1.66, (this->infoconfig->alto / 2.3));
		}
	}else {
		cout<<"No es una imagen corecta"<<endl;
	}

}

list<Carta>* Juego::pedirCartas(){
	string idOperacion="C";
	list<string>* operandos=new list<string>();
	char* xml = parser->getXmlDeOperacion(idOperacion, operandos);
	cliente->enviar(xml);
	char * respuesta = cliente->recibirRespuesta();
	list<string>* cartasAux = parserResultado->getCartas(respuesta);
	list<string>::iterator it;
	list<Carta> * cartas = new list<Carta> ();
	list<Carta>::iterator iterador = cartas->begin();
	it = cartasAux->begin();
	for (unsigned int i = 0; i < cartasAux->size() / 2; i++) {
		string palo = (*it);
		it++;
		string numero = (*it);
		it++;
		iterador = cartas->insert(iterador,
				Carta("", palo, numero, i + 1));
		iterador++;
	}
	this->cartas = cartas;
	return cartas;
//	list<string>::iterator it;
//	it = cartas->begin();
//	for (unsigned int i = 0; i < cartas->size(); i++) {
//		cout << *it << endl;
//		it++;
//	}
}

void Juego::dibujarBoton(string textoBoton, int pos){
	BitMap* boton = new BitMap("quilmes.bmp");
	if (boton->esUnaImagenCorrecta()) {
		int tamImagenancho = this->infoconfig->ancho / 6.3;
		int tamImagenalto = this->infoconfig->alto / 5;
		int tamfuente = this->infoconfig->ancho / 42;
		int factor = this->infoconfig->ancho / 100;
		const char* texto = textoBoton.c_str();
		boton->resizeTo(tamImagenalto, tamImagenancho);
		if (pos == 1) {
			pantalla->dibujarBitMapDesdePos(*boton, this->infoconfig->ancho / 2, (this->infoconfig->alto / 1.3));
			pantalla->escribirTextoDesdePos(texto,(this->infoconfig->ancho / 2)+factor ,(this->infoconfig->alto / 1.3)+(tamImagenalto/2)-(tamfuente/2), tamfuente);
		} else if (pos == 2) {
			pantalla->dibujarBitMapDesdePos(*boton, this->infoconfig->ancho / 1.5, (this->infoconfig->alto / 1.3));
			pantalla->escribirTextoDesdePos(texto,(this->infoconfig->ancho / 1.5)+factor ,(this->infoconfig->alto / 1.3)+(tamImagenalto/2)-(tamfuente/2), tamfuente);
		} else if (pos == 3) {
			pantalla->dibujarBitMapDesdePos(*boton, this->infoconfig->ancho / 1.18, (this->infoconfig->alto / 1.3));
			pantalla->escribirTextoDesdePos(texto,(this->infoconfig->ancho / 1.18)+factor ,(this->infoconfig->alto / 1.3)+(tamImagenalto/2)-(tamfuente/2), tamfuente);
		}
	} else {
		cout << "No es una imagen corecta" << endl;
	}

}

void Juego::dibujarPoso(){
	int tamfuente = this->infoconfig->ancho / 28;
	string pozo = "BOTE $: " + this->posoAcumulado;
	const char* bote = pozo.c_str();
	pantalla->escribirTextoDesdePos(bote, this->infoconfig->ancho / 2.5 ,(this->infoconfig->alto / 56) , tamfuente);

}

void Juego::pedirPoso(){
	string idOperacion="P";
	list<string>* operandos=new list<string>();
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	cliente->enviar(xml);
	char * respuesta=cliente->recibirRespuesta();
	posoAcumulado=parserResultado->getPoso(respuesta);
}

void Juego::actualizarPantalla(){
	this->pantalla->actualizarPantalla(0,0,0,0);
}
