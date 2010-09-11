/*
 * ParserResultadoCliente.cpp
 *
 *  Created on: 30/08/2010
 *      Author: gaston
 */

#include "ParserResultadoCliente.h"

ParserResultadoCliente::ParserResultadoCliente() {
	// TODO Auto-generated constructor stub

}

void ParserResultadoCliente::DecodificaResultado(char* xml){
	char* buffer = strtok(xml,"<>");
	buffer = strtok(NULL,"<>");
	buffer = strtok(NULL,"<>");
	string tipo;
	char* palabraclave;
	if (strcmp(buffer, "<resultados>") == 0) {
		tipo = "El resultado de: ";
		palabraclave = "nombre";
	} else {
		tipo = "El error fue de tipo: ";
		palabraclave = "tipo";
	}
	while (buffer != NULL) {
		if (strcmp(buffer, palabraclave) == 0) {
			buffer = strtok(NULL, "\t <>=\"");
			cout << tipo << buffer << " es ";
			buffer = strtok(NULL, "<>\t");
			cout << buffer << endl;
		}
		buffer = strtok(NULL, " \t<>=");

	}
	this->registrarResultado(xml);
}

void ParserResultadoCliente::registrarResultado(char* xml){
	this->archivoResultado->open("respuesta.txt", ios::out | ios::app);
	string lineaActual;
	this->archivoResultado >> lineaActual;
	while(this->archivoResultado >> lineaActual){
		cout<<lineaActual<<endl;
	}

}

ParserResultadoCliente::~ParserResultadoCliente() {
	// TODO Auto-generated destructor stub
}
