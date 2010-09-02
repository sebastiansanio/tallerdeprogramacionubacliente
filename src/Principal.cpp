using namespace std;
#include <string>
#include <stdlib.h>
#include <SolarSockets/SolarSockets++.h>
#include <ctype.h>
#include "Cliente.h"

int main(){

	Cliente* cliente=new Cliente();
	string server;
	int port;
	bool conectado=false;
	bool salir=false;

	//Para conectarse con un servidor deseado por el usuario
	while((!conectado)or(salir)){
		system("cls");
		string respuesta;
		cout<<"Ingrese servidor"<<endl;
		cin>>server;
		cout<<"Ingrese Puerto"<<endl;
		cin>>port;
		conectado=cliente->Connect(server,port);
		if(!(conectado)){
			cout<<"Error al conectar"<<endl;
			cout<<"Desea salir? S/N"<<endl;
			cin>>respuesta;
			respuesta=toupper(respuesta[0]);
			salir=((respuesta=="S"));
		}
	}
	bool seguir=conectado;
	//Si se logro conectar empieza la interacción usuario - cliente - servidor
	while(seguir){
		system("cls");
		cout<<"Elija la opción deseada:"<<endl;
		cout<<"1 - Ingresar operacion"<<endl;
		cout<<"2 - Levantar archivo de operaciones"<<endl;
		cout<<"0 - Salir"<<endl;
		string respuesta;
		cin>>respuesta;
		while((respuesta!="1")or(respuesta!="2")or(respuesta!="0")){
				cout<<"Ingrese una opción valida"<<endl;
				cin>>respuesta;
		}
		switch(respuesta[0]){
			case '1':cliente->enviarOperacion(); break;
			case '2':break;
			case '0': seguir=false; break;
		}
	}
	return 0;
}
