using namespace std;
#include <string>
#include <stdlib.h>
#include <SolarSockets/SolarSockets++.h>
#include <ctype.h>
#include "Cliente.h"
#include <unistd.h>

int main(){
	Cliente* cliente= new Cliente();
	cliente->conectar();
	bool seguir=true;
	//empieza la interacción usuario - cliente - servidor
	while(seguir){
		system("clear");
		cout<<"Elija la opción deseada:"<<endl;
		cout<<"1 - Ingresar operacion"<<endl;
		cout<<"2 - Levantar archivo de operaciones"<<endl;
		//cout<<"3 - Entrar a la prueba"<<endl;
		cout<<"0 - Salir"<<endl;
		string respuesta;
		cin>>respuesta;
		while((respuesta!="1")and(respuesta!="2")and(respuesta!="0")){
				cout<<"Ingrese una opción valida"<<endl;
				cin>>respuesta;
		}
		switch(respuesta[0]){
			case '1':cliente->interactuarConUsuarioYservidor();break;
			case '2':{
				string nombreArchivo;
				cout<<"Ingrese el nombre del archivo"<<endl;
				cin>>nombreArchivo;
				cliente->enviarArchivoOperaciones(nombreArchivo);
			}
				break;
			//case '3':prueba(cliente);break;
			case '0': seguir=false; break;
		}
	}
	delete cliente;
	return 0;
}