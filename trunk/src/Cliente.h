#ifndef CLIENTE_H_
#define CLIENTE_H_

using namespace std;
#include "ParserResultadoCliente.h"
#include "ParserCliente.h"
#include "Pantalla.h"
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
#include <sstream>

class Cliente{
private:
	ParserResultadoCliente* parserResultado;
	ParserCliente* parser;
	sockaddr_in estructuraDeDireccion;
	int descriptorSocket;
	list<string> * jugadores;
public:
	Cliente();
	Cliente(string archivoConfig);
	void conectar();
	void enviar(char data[]);
	void interactuarConUsuarioYservidor();
	void enviarArchivoOperaciones(string nombreArchivo);
	void recibir();
	string recibirArchivo();//Devuelve el path donde lo guardo
	virtual ~Cliente();
};

#endif /* CLIENTE_H_ */
