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
	string operacion = buffer;
	buffer = strtok(NULL,"<>");
	string tipo;
	char* palabraclave;

	//Registro el resultado en el archivo correspondiente
	if (strcmp(buffer, "<resultados>") == 0) {
		tipo = "El resultado de: ";
		palabraclave = "nombre";
		this->registrarResultado(xml,operacion,"resultados","resultados");
	} else {
		tipo = "El error fue de tipo: ";
		palabraclave = "tipo";
		this->registrarResultado(xml,operacion,"errores","errores.err");
	}

	/*
	//Reinicio el buffer para imprimir por pantalla los errores o resultados
	buffer = strtok(xml,"<>");
	buffer = strtok(NULL,"<>");
	buffer = strtok(NULL,"<>");

	while (buffer != NULL) {
		if (strcmp(buffer, palabraclave) == 0) {
			buffer = strtok(NULL, "\t <>=\"");
			cout << tipo << buffer << " es ";
			buffer = strtok(NULL, "<>\t");
			cout << buffer << endl;
		}
		buffer = strtok(NULL, " \t<>=");
	}
	*/
}

void ParserResultadoCliente::registrarResultado(char xml [],string operacion, string tipo, const char* archivo){
	(this->archivoResultado) = new ofstream(archivo, ios::out | ios::app);
	string lineaActual = xml;
	*this->archivoResultado << "<respuesta>" << endl;
	*this->archivoResultado << "\t<" << operacion << ">" << endl;
	*this->archivoResultado << "\t\t<" << tipo << ">" << endl;
	lineaActual = strtok(NULL, "<>\t\n");
	while (lineaActual != "/resultados") {
		*this->archivoResultado << "\t\t\t<" << lineaActual << ">" << endl;
		lineaActual = strtok(NULL, "<>\t\n");
		*this->archivoResultado << "\t\t\t" << lineaActual << endl;
		lineaActual = strtok(NULL, "<>\t\n");
		*this->archivoResultado << "\t\t\t<" << lineaActual << ">" << endl;
		lineaActual = strtok(NULL, "<>\t\n");
	}
	*this->archivoResultado << "\t\t</" << tipo << ">" << endl;
	*this->archivoResultado << "</respuesta>" << endl;
	this->archivoResultado->close();

}

ParserResultadoCliente::~ParserResultadoCliente() {
	// TODO Auto-generated destructor stub
	delete this->archivoResultado;
}
