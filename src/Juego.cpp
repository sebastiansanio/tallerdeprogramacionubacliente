#include "Juego.h"

Juego::Juego() {
	this->parser=new ParserCliente();
	this->parserResultado=new ParserResultadoCliente();
	ParserCliente *parserAux=new ParserCliente(PATHARCHIVOCONF);
	this->pantalla = new Pantalla(ALTO,ANCHO);
	if(parserAux->comprobarSintaxis()){
//		int* altoYancho=parserAux->getAltoYAnchoDeConfig();
//		//this->pantalla=new Pantalla(altoYancho[0],altoYancho[1]);
//		cout << "alto y ancho" << endl;
//		cout << altoYancho[0] << endl;
//		cout << altoYancho[1] << endl;
//		delete []altoYancho;
//		int* puertoYIP = parserAux->getPuertoYIP();
//		cout << "puerto e ip" << endl;
//		cout << puertoYIP[0] << endl;
//		cout << puertoYIP[1] << endl;
	}
	else {
		cout << "Sintaxis incorrecta" << endl;
	}
//	this->cliente=new Cliente(puertoYIP[0],puertoYIP[1]);
	this->cliente = new Cliente();
	cliente->conectar();
//	delete parserAux;
//	delete puertoYIP;
}

Juego::~Juego() {
	delete pantalla;
	delete cliente;
	delete parser;
	delete parserResultado;
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
		escenario->resizeTo(ALTO,ANCHO);
		pantalla->dibujarBitMapDesdePos((*escenario),0,0);
	}else{
		cout<<"No es una imagen corecta"<<endl;
	}
}


void Juego::dibujarJugador(int x, int y, Jugador jugadorADibujar){
	BitMap* jugador=new BitMap(jugadorADibujar.getPath());
	//dibujaria con el sdl_ttf el nombre y la plata al lado de la imagen
	if(jugador->esUnaImagenCorrecta()){
		jugador->resizeTo(100,100);
		pantalla->dibujarBitMapDesdePos((*jugador),x,y);
	}else{
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
		iterador=jugadores->insert(iterador,Jugador("",nombre,plata));
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
