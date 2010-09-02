#ifndef CLIENTE_H_
#define CLIENTE_H_

using namespace std;
#include "ParserResultadoCliente.h"
#include "ParserCliente.h"
#include <SolarSockets/SolarSockets++.h>
#include <string>
#include <stdlib.h>
#include <ctype.h>

class Cliente : public ssPPClient
{
private:
	ParserResultadoCliente* parserResultado;
	ParserCliente* parser;
public:
	Cliente();
	virtual void onConnect();
	virtual void onDataArrival(string Data);
	virtual void enviarOperacion();
	virtual ~Cliente();
};

#endif /* CLIENTE_H_ */
