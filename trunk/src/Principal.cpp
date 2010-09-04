using namespace std;
#include <string>
#include <stdlib.h>
#include <SolarSockets/SolarSockets++.h>
#include <ctype.h>
#include "Cliente.h"
#include <unistd.h>

int main(){

//	Cliente* cliente=new Cliente();
//	string server;
//	int port;
//	bool conectado=false;
//	bool salir=false;
//
//	//Para conectarse con un servidor deseado por el usuario
//	while((!conectado)or(salir)){
//		system("clear");
//		string respuesta;
//		cout<<"Ingrese servidor"<<endl;
//		cin>>server;
//		cout<<"Ingrese Puerto"<<endl;
//		cin>>port;
//		conectado=cliente->Connect(server,port);
//		sleep(10);
//		if(!(conectado)){
//			cout<<"Error al conectar"<<endl;
//			cout<<"Desea salir? S/N"<<endl;
//			cin>>respuesta;
//			respuesta=toupper(respuesta[0]);
//			salir=((respuesta=="S"));
//		}
//	}
//	bool seguir=conectado;
//	//Si se logro conectar empieza la interacción usuario - cliente - servidor
//	while(seguir){
//		system("clear");
//		cout<<"Elija la opción deseada:"<<endl;
//		cout<<"1 - Ingresar operacion"<<endl;
//		cout<<"2 - Levantar archivo de operaciones"<<endl;
//		cout<<"0 - Salir"<<endl;
//		string respuesta;
//		cin>>respuesta;
//		while((respuesta!="1")and(respuesta!="2")and(respuesta!="0")){
//				cout<<"Ingrese una opción valida"<<endl;
//				cin>>respuesta;
//		}
//		switch(respuesta[0]){
//			case '1':cliente->enviarOperacion(); break;
//			case '2':break;
//			case '0': seguir=false; break;
//		}
//	}
	Cliente* cliente= new Cliente();
	cliente->conectar();
	char data[MAXBYTES]="Holaaaaaaaaaaaaaaaaa vamossssssssss que andaaaa";
	cliente->enviar(data);
	sleep(1);
	cliente->recibir();
	return 0;
}
