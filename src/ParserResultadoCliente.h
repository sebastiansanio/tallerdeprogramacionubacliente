#ifndef PARSERRESULTADOCLIENTE_H_
#define PARSERRESULTADOCLIENTE_H_

using namespace std;
#include <string>
#include <fstream>
class ParserResultadoCliente {
private:
	ofstream* archivoResultado;
public:
	ParserResultadoCliente();
	void DecodificaResultado(char* xml);
	virtual ~ParserResultadoCliente();
	void registrarResultado(char* xml);
};

#endif /* PARSERRESULTADOCLIENTE_H_ */
