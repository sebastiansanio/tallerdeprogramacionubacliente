#ifndef CLIENTE_H_
#define CLIENTE_H_

using namespace std;
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
#include <string>
#include <fstream>
#include <ctype.h>
#include <list>
#include "ParserResultadoCliente.h"

class Cliente{
private:
	sockaddr_in estructuraDeDireccion;
	int descriptorSocket;
public:
	Cliente();
	Cliente(string archivoConfig);
	Cliente(unsigned int puerto, string IP);
	void conectar();
	void enviar(char data[]);
	char * recibirRespuesta();
	string recibirArchivo(string path);//Devuelve el path donde lo guardo
	virtual ~Cliente();
};

#endif /* CLIENTE_H_ */
