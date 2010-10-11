#include "Juego.h"

Juego::Juego() {
	this->parser=new ParserCliente();
	this->parserResultado=new ParserResultadoCliente();
	ParserCliente *parserAux=new ParserCliente(PATHARCHIVOCONF);

	if(parserAux->comprobarSintaxis()){
//		int* altoYancho=parserAux->getAltoYAnchoDeConfig();
//		//this->pantalla=new Pantalla(altoYancho[0],altoYancho[1]);
//		cout << "alto y ancho" << endl;
//		cout << altoYancho[0] << endl;
//		cout << altoYancho[1] << endl;
//		delete []altoYancho;
//		string* puertoYIP = parserAux->getPuertoYIP();
//		cout << "puerto e ip" << endl;
//		cout << puertoYIP[0] << endl;
//		cout << puertoYIP[1] << endl;
//		delete []puertoYIP;

		this->infoconfig = parserAux->getInformacionConfig();
		cout << "alto y ancho" << endl;
		cout << this->infoconfig->alto << endl;
		cout << this->infoconfig->ancho << endl;
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


void Juego::dibujarJugador(int x, int y, Jugador jugadorADibujar){
	BitMap* jugador = new BitMap(jugadorADibujar.getPath());
	//dibujaria con el sdl_ttf el nombre y la plata al lado de la imagen
	if (jugador->esUnaImagenCorrecta()) {
		jugador->resizeTo(this->infoconfig->ancho / 10, this->infoconfig->ancho / 10);
		if (jugadorADibujar.getId() == 1) {
			pantalla->dibujarBitMapDesdePosCircular(*jugador,
					this->infoconfig->ancho / 8, this->infoconfig->alto / 2);
		} else if (jugadorADibujar.getId() == 2) {
			pantalla->dibujarBitMapDesdePosCircular(*jugador,
					this->infoconfig->ancho / 5.4, this->infoconfig->alto / 3.2);
		} else if (jugadorADibujar.getId() == 3) {
			pantalla->dibujarBitMapDesdePosCircular(*jugador,
					this->infoconfig->ancho / 2.8, this->infoconfig->alto / 3.9);
		} else if (jugadorADibujar.getId() == 4) {
			pantalla->dibujarBitMapDesdePosCircular(*jugador,
					this->infoconfig->ancho / 1.5, this->infoconfig->alto / 3.9);
		} else if (jugadorADibujar.getId() == 5) {
			pantalla->dibujarBitMapDesdePosCircular(*jugador,
					this->infoconfig->ancho / 1.4, this->infoconfig->alto / 3.2);
		} else if (jugadorADibujar.getId() == 6) {
			pantalla->dibujarBitMapDesdePosCircular(*jugador,
					this->infoconfig->ancho / 1.3, this->infoconfig->alto / 2);
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
	return jugadores;
}


void Juego::pedirCartas(){
	string idOperacion="C";
	list<string>* operandos=new list<string>();
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	cliente->enviar(xml);
	char * respuesta=cliente->recibirRespuesta();
	cartas = parserResultado->getCartas(respuesta);
//	list<string>::iterator it;
//	it = cartas->begin();
//	for (unsigned int i = 0; i < cartas->size(); i++) {
//		cout << *it << endl;
//		it++;
//	}
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
