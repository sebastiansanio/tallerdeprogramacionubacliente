#ifndef PARSERRESULTADOCLIENTE_H_
#define PARSERRESULTADOCLIENTE_H_

#define PORT 54340
#define MAXBYTES 60000
#define MAXBYTESRECIBIDOS 6000

using namespace std;
#include <string>
#include <fstream>
#include <string.h>
#include <sstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <ctype.h>
#include <list>


class ParserResultadoCliente {
private:
	ofstream* archivoResultado;
public:
	ParserResultadoCliente();
	list<string> * getJugadores(char xml[]);//Le pasamos el xml de la operacion J y nos da los jugadores que no paso el servidor que estaban
	list<string> * getCartas(char xml[]);
	string getPoso(char xml[]);
	void DecodificaResultado(char* xml);
	virtual ~ParserResultadoCliente();
	void registrarResultado(char xml [],string operacion, string tipo, const char* archivo);
};

#endif /* PARSERRESULTADOCLIENTE_H_ */
