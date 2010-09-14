#ifndef PARSERRESULTADOCLIENTE_H_
#define PARSERRESULTADOCLIENTE_H_

#define PORT 54340
#define MAXBYTES 3000

using namespace std;
#include <string>
#include <fstream>
#include <string.h>
#include <iostream>


class ParserResultadoCliente {
private:
	ofstream* archivoResultado;
public:
	ParserResultadoCliente();
	void DecodificaResultado(char* xml);
	virtual ~ParserResultadoCliente();
	void registrarResultado(char xml [],string operacion, string tipo, const char* archivo);
};

#endif /* PARSERRESULTADOCLIENTE_H_ */
