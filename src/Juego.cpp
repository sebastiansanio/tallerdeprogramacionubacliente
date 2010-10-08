#include "Juego.h"

Juego::Juego() {
	this->pantalla = new Pantalla(ALTO,ANCHO);
	this->cliente = new Cliente();
	cliente->conectar();
	this->parser=new ParserCliente();
	this->parserResultado=new ParserResultadoCliente();
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

//void Juego::pedirJugadores(){
//	string idOperacion="J";
//	list<string>* operandos=new list<string>();
//	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
//	delete operandos;
//	cliente->enviar(xml);
//	char * respuesta=cliente->recibirRespuesta();
//	list<string> * jugadores=parserResultado->getJugadores(respuesta);
//}
//
//void Juego::pedirCartas(){
//	string idOperacion="C";
//	list<string>* operandos=new list<string>();
//	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
//	cliente->enviar(xml);
//	char * respuesta=cliente->recibirRespuesta();
//	list<string> * cartas=parserResultado->getCartas(respuesta);
//}
//
//void Juego::pedirPoso(){
//	string idOperacion="P";
//	list<string>* operandos=new list<string>();
//	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
//	cliente->enviar(xml);
//	char * respuesta=cliente->recibirRespuesta();
//	string posoAcumulado=parserResultado->getPoso(respuesta);
//}


