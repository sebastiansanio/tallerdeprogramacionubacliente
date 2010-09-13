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

void ParserResultadoCliente::DecodificaResultado(char xml []){

	char* buffer = strtok(xml,"<>");
	buffer = strtok(NULL,"<>");
	buffer = strtok(NULL,"<>");
	string tipo;
	char* palabraclave;

	//Registro el resultado en el archivo correspondiente
	if (strcmp(buffer, "<resultados>") == 0) {
		tipo = "El resultado de: ";
		palabraclave = "nombre";
		this->registrarResultado(xml,"resultados");
	} else {
		tipo = "El error fue de tipo: ";
		palabraclave = "tipo";
		this->registrarResultado(xml,"errores.err");
	}
	/*
	char* buffer = strtok(xml,"<>");
	buffer = strtok(NULL,"<>");
	buffer = strtok(NULL,"<>");
	*/
	while (buffer != NULL) {
		if (strcmp(buffer, palabraclave) == 0) {
			buffer = strtok(NULL, "\t <>=\"");
			cout << tipo << buffer << " es ";
			buffer = strtok(NULL, "<>\t");
			cout << buffer << endl;
		}
		buffer = strtok(NULL, " \t<>=");

	}

}

void ParserResultadoCliente::registrarResultado(char xml [], const char* archivo){
	(this->archivoResultado) = new ofstream(archivo, ios::out | ios::app);
	string lineaActual = xml;
	(*this->archivoResultado) << lineaActual << endl;
	while(lineaActual != ""){
		//Falta como pasar el xml [] al archivo de forma correcta
		//lo que esta aca esta mal, escribe todo mal tabulado y ademas
		//continua donde habia quedado el xml cuando lo pasan por parametro
		//porque el strtok elimina a medida q avanza, asi que hay q tener cuidado
		cout<<lineaActual<<endl; // CAMBIAR
		lineaActual = strtok(NULL, "<>"); // CAMBIAR
		*this->archivoResultado << lineaActual << endl; // CAMBIAR

	}
	this->archivoResultado->close();

}

ParserResultadoCliente::~ParserResultadoCliente() {
	// TODO Auto-generated destructor stub
	delete this->archivoResultado;
}
