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

string Juego::pedirImagenJugador(string nombreJugador){
	string ruta=nombreJugador + ".bmp";
	//Pido el escenario
	string idOperacion="I";
	list<string>* operandos=new list<string>();
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


void Juego::dibujarJugador(int x, int y, string path){
	BitMap* jugador=new BitMap(path);
	if(jugador->esUnaImagenCorrecta()){
		jugador->resizeTo(100,100);
		pantalla->dibujarBitMapDesdePos((*jugador),x,y);
	}else{
		cout<<"No es una imagen corecta"<<endl;
	}
}


void Juego::pedirJugadores(){
	string idOperacion="J";
	list<string>* operandos=new list<string>();
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	delete operandos;
	cliente->enviar(xml);
	char * respuesta=cliente->recibirRespuesta();
	jugadores=parserResultado->getJugadores(respuesta);
//	list<string>::iterator it;
//	it = jugadores->begin();
//	for(unsigned int i = 0; i < jugadores->size(); i++){
//		cout << *it << endl;
//		it++;
//	}

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
