#ifndef PARSERCLIENTE_H_
#define PARSERCLIENTE_H_

using namespace std;
#include <string>
#include <list>
#include <iostream>
#include <fstream>

class ParserCliente {
private:
	ifstream* archivo;
public:
	ParserCliente();
	ParserCliente(const char* archivoXml);
	const char* getSiguienteOperacion();
	const char* getXmlDeOperacion(string idOperacion, list<string>* operandos);
	virtual ~ParserCliente();
};

#endif /* PARSERCLIENTE_H_ */
