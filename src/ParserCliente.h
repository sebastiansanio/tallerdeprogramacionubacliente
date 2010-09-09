#ifndef PARSERCLIENTE_H_
#define PARSERCLIENTE_H_

using namespace std;
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include "Grafo.h"

class ParserCliente {
private:
	ifstream* archivo;
	ofstream* archivoerrores;
	bool fallido;
	Grafo* grafoTags;

public:
	ParserCliente();
	ParserCliente(const char* archivoXml);
	const char* getSiguienteOperacion();
	bool comprobarSintaxis();
	bool comprobarTag(string* cadenaArchivo,string* cadenaNodo);
	const char* getXmlDeOperacion(string idOperacion, list<string>* operandos);
	void errorSintaxis(string* cadenaArchivo,string* cadenaNodo);
	void registrarError(string idOperacion, list<string>* mensajesError);
	virtual ~ParserCliente();
	void construirGrafo();
	string devolverUnPedido();
};

#endif /* PARSERCLIENTE_H_ */
