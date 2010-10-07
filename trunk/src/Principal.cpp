using namespace std;
#define PATH "/home/gaston/workspace/TpTallerDeProgramacionICliente/config.ini"
#include <string>
#include <stdlib.h>
#include <ctype.h>
#include "Cliente.h"
#include <unistd.h>
#include "Pantalla.h"
#include "math.h"
#include "BitMap.h"
#include "ParserResultadoCliente.h"

int main(){
	Cliente* cliente= new Cliente();
//	Cliente * cliente=new Cliente(PATH); IRIA ESTEEEEEEE
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
			case '1':{cliente->interactuarConUsuarioYservidor();break;}
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
					string ruta="/home/gaston/workspace/TpTallerDeProgramacionICliente/pantalla.bmp";
					ParserCliente *parser=new ParserCliente();
					ParserResultadoCliente * parserResultado=new ParserResultadoCliente();

					//Pido el escenario
					string idOperacion="E";
					list<string>* operandos=new list<string>();
					char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
					cliente->enviar(xml);
					string path=cliente->recibirArchivo(ruta);
					BitMap* escenario=new BitMap(ruta);
					delete operandos;
/*					//Pido los jugadores
					string idOperacion="J";
					list<string>* operandos=new list<string>();
					char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
					cliente->enviar(xml);
					char * respuesta=cliente->recibirRespuesta();
					list<string> * jugadores=parserResultado->getJugadores(respuesta);

					//Pido cartas en juego
					string idOperacion="C";
					list<string>* operandos=new list<string>();
					char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
					cliente->enviar(xml);
					char * respuesta=cliente->recibirRespuesta();
					list<string> * cartas=parserResultado->getCartas(respuesta);

					//Pido el poso acumulado
					string idOperacion="P";
					list<string>* operandos=new list<string>();
					char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
					cliente->enviar(xml);
					char * respuesta=cliente->recibirRespuesta();
					string posoAcumulado=parserResultado->getPoso(respuesta);*/

					//Empiezo a mostrar
					Pantalla* pantalla=new Pantalla(alto,ancho);
					if(escenario->esUnaImagenCorrecta()){
						escenario->resizeTo(alto,ancho);
						pantalla->dibujarBitMapDesdePos((*escenario),0,0);
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
