#ifndef PARSERRESULTADOCLIENTE_H_
#define PARSERRESULTADOCLIENTE_H_

using namespace std;
#define PORT 54340
#define MAXBYTES 60000
#define MAXBYTESRECIBIDOS 60000
#include <string>
#include <fstream>
#include <string.h>
#include <sstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <ctype.h>
#include <list>
#include "Grafo.h"


class ParserResultadoCliente {
private:
	ofstream* archivoResultado;
public:
	ParserResultadoCliente();
	list<string> * getJugadores(char xml[]);//Le pasamos el xml de la operacion J y nos da los jugadores que no paso el servidor que estaban
	bool getPlataYCargado(char xml[],int* plata, int* cargado);
	list<string> * getCartas(char xml[]);
	string getPoso(char xml[]);
	bool DecodificaResultado(char* xml);
	virtual ~ParserResultadoCliente();
	void registrarResultado(char xml [],string operacion, string tipo, const char* archivo);
};

#endif /* PARSERRESULTADOCLIENTE_H_ */
