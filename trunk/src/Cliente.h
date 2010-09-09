#ifndef CLIENTE_H_
#define CLIENTE_H_

#define PORT 54340
#define MAXBYTES 300

using namespace std;
#include "ParserResultadoCliente.h"
#include "ParserCliente.h"
#include <string>
#include <ctype.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>

class Cliente{
private:
	ParserResultadoCliente* parserResultado;
	ParserCliente* parser;
	sockaddr_in estructuraDeDireccion;
	int descriptorSocket;
public:
	Cliente();
	void conectar();
	void enviar(char data[]);
	void enviarOperacion();
	void enviarArchivoOperaciones(string nombreArchivo);
	void recibir();
	virtual ~Cliente();
};

#endif /* CLIENTE_H_ */
