#include "Juego.h"

Juego::Juego() {
	this->parser=new ParserCliente();
	this->parserResultado=new ParserResultadoCliente();
//	ParserCliente *parserAux=new ParserCliente(PATHARCHIVOCONF);
//	if(parserAux->comprobarSintaxis()){
//		char * altoYancho=parserAux->getAltoYAnchoDeConfig();
//		this->pantalla=new Pantalla(altoYancho[0],altoYancho[1]);
//		this->pantalla = new Pantalla(ALTO,ANCHO);
//		delete []altoYancho;
//	}
	//	int * puertoYIP=this->parserAux->getPuertoYIP();
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

void Juego::pedirEscenario(){
	string ruta=PATHESCENARIO;
	//Pido el escenario
	string idOperacion="E";
	list<string>* operandos=new list<string>();
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	delete operandos;
	cliente->enviar(xml);
	cliente->recibirArchivo(ruta);
}

void Juego::dibujarEscenario(){
	BitMap* escenario=new BitMap(PATHESCENARIO);
	if(escenario->esUnaImagenCorrecta()){
		escenario->resizeTo(ALTO,ANCHO);
		pantalla->dibujarBitMapDesdePos((*escenario),0,0);
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
	cout<<"hola"<<endl;
	char * respuesta=cliente->recibirRespuesta();
	cout<<"Jugadores"<<endl;
	cout<<respuesta<<endl;
//	jugadores=parserResultado->getJugadores(respuesta);
}

void Juego::pedirCartas(){
	string idOperacion="C";
	list<string>* operandos=new list<string>();
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	cliente->enviar(xml);
	cout<<"hola3"<<endl;
	char * respuesta=cliente->recibirRespuesta();
	cout<<"Cartas"<<endl;
//	cartas=parserResultado->getCartas(respuesta);
}

void Juego::pedirPoso(){
	string idOperacion="P";
	list<string>* operandos=new list<string>();
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	cliente->enviar(xml);
	cout<<"hola2"<<endl;
	char * respuesta=cliente->recibirRespuesta();
	cout<<"Poso"<<endl;
	cout<<respuesta<<endl;
//	posoAcumulado=parserResultado->getPoso(respuesta);
}
