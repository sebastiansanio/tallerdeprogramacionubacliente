using namespace std;
#include <string>
#include <stdlib.h>
#include <SolarSockets/SolarSockets++.h>
#include <ctype.h>
#include "Cliente.h"
#include <unistd.h>
#include "Pantalla.h"
#include "math.h"
#include "BitMap.h"

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
		cout<<"3 - Pruebas de pantalla"<<endl;
		cout<<"0 - Salir"<<endl;
		string respuesta;
		cin>>respuesta;
		while((respuesta!="1")and(respuesta!="2")and(respuesta!="0")and(respuesta!="3")){
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
			case '3':{
					int alto=600;
					int ancho=800;
					ParserCliente *parser=new ParserCliente();
					string idOperacion="B";
					list<string>* operandos=new list<string>();
					char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
					cliente->enviar(xml);
					string path=cliente->recibirArchivo();
					Pantalla* pantalla=new Pantalla(alto,ancho);
					BitMap* bitmap=new BitMap(path);
					bitmap->resizeTo(alto,ancho);
					if(bitmap->esUnaImagenCorrecta()){
						pantalla->dibujarBitMapDesdePos((*bitmap),0,0);
					}else{
						cout<<"No es una imagen corecta"<<endl;
					}
					SDL_Event evento;
					bool terminar=false;
					while(!terminar){
						while(SDL_PollEvent(&evento)) {
							if((evento.type == SDL_QUIT)){
								terminar=true;
								break;
							}
						}
					}
					delete pantalla;
					break;
			}
			case '0': seguir=false; break;
		}
	}
	delete cliente;
	return 0;
}
