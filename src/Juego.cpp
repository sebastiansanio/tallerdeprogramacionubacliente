#include "Juego.h"

void * manejoEventos(void * juego_aux){
	Juego * juego=(Juego *)juego_aux;
	int x_nombre_jugador=5,y_nombre_jugador=220;
	SDL_Color rojo;
	rojo.r=255;
	rojo.g=0;
	rojo.b=0;
	SDL_Color blanco;
	blanco.r=255;
	blanco.g=255;
	blanco.b=255;
	bool jugador_observador=true;
	while(true){
		SDL_Event evento;
		while(SDL_PollEvent(&evento)){
			//Hago lo que tenga que hacer si es algo de jugar o apostar primero pregunto si es mi turno
			if(evento.type == SDL_QUIT){
				exit(0);
			}else if(evento.type == SDL_MOUSEBUTTONDOWN){
				if(evento.button.button==1){
					cout<<"x: "<<evento.button.x<<"  y: "<<evento.button.y<<endl;
					//Si me pidio alguna carta de algun jugador
					if(jugador_observador){
						if(evento.button.x>=x_nombre_jugador and evento.button.x<=(x_nombre_jugador + 140)){
							if(evento.button.y<=(y_nombre_jugador) and evento.button.y>=35){
								if(evento.button.y>35 and evento.button.y<=65){
									Jugador * jugador_aux=juego->getJugador(1);
									if(jugador_aux!=NULL){
										if(jugador_aux->getCartas()!=NULL){
											juego->pantalla->escribirTextoDesdePos(jugador_aux->getNombre().c_str(),x_nombre_jugador,40,30,rojo);
											juego->actualizarPantalla();
											juego->dibujarCartaJugador(jugador_aux);
										}else{
											juego->pantalla->escribirTextoDesdePos("El jugador no posee cartas",5,juego->infoconfig->alto*(0.95),24,rojo);
											juego->actualizarPantalla();
											sleep(1);
										}
									}
								}else if(evento.button.y>65 and evento.button.y<=95){
									Jugador * jugador_aux=juego->getJugador(2);
									if(jugador_aux!=NULL){
										if(jugador_aux->getCartas()!=NULL){
											juego->pantalla->escribirTextoDesdePos(jugador_aux->getNombre().c_str(),x_nombre_jugador,70,30,rojo);
											juego->actualizarPantalla();
											juego->dibujarCartaJugador(jugador_aux);
										}else{
											juego->pantalla->escribirTextoDesdePos("El jugador no posee cartas",5,juego->infoconfig->alto*(0.95),24,rojo);
											juego->actualizarPantalla();
											sleep(1);
										}
									}
								}else if(evento.button.y>95 and evento.button.y<=125){
									Jugador * jugador_aux=juego->getJugador(3);
									if(jugador_aux!=NULL){
										if(jugador_aux->getCartas()!=NULL){
											juego->pantalla->escribirTextoDesdePos(jugador_aux->getNombre().c_str(),x_nombre_jugador,100,30,rojo);
											juego->actualizarPantalla();
											juego->dibujarCartaJugador(jugador_aux);
										}else{
											juego->pantalla->escribirTextoDesdePos("El jugador no posee cartas",5,juego->infoconfig->alto*(0.95),24,rojo);
											juego->actualizarPantalla();
											sleep(1);
										}
									}
								}else if(evento.button.y>125 and evento.button.y<=155){
									Jugador * jugador_aux=juego->getJugador(4);
									if(jugador_aux!=NULL){
										if(jugador_aux->getCartas()!=NULL){
											juego->pantalla->escribirTextoDesdePos(jugador_aux->getNombre().c_str(),x_nombre_jugador,130,30,rojo);
											juego->actualizarPantalla();
											juego->dibujarCartaJugador(jugador_aux);
										}else{
											juego->pantalla->escribirTextoDesdePos("El jugador no posee cartas",5,juego->infoconfig->alto*(0.95),24,rojo);
											juego->actualizarPantalla();
											sleep(1);
										}
									}
								}else if(evento.button.y>155 and evento.button.y<=185){
									Jugador * jugador_aux=juego->getJugador(5);
									if(jugador_aux!=NULL){
										if(jugador_aux->getCartas()!=NULL){
											juego->pantalla->escribirTextoDesdePos(jugador_aux->getNombre().c_str(),x_nombre_jugador,160,30,rojo);
											juego->actualizarPantalla();
											juego->dibujarCartaJugador(jugador_aux);
										}else{
											juego->pantalla->escribirTextoDesdePos("El jugador no posee cartas",5,juego->infoconfig->alto*(0.95),24,rojo);
											juego->actualizarPantalla();
											sleep(1);
										}
									}
								}else if(evento.button.y>185 and evento.button.y<=220){
									Jugador * jugador_aux=juego->getJugador(6);
									if(jugador_aux!=NULL){
										if(jugador_aux->getCartas()!=NULL){
											juego->pantalla->escribirTextoDesdePos(jugador_aux->getNombre().c_str(),x_nombre_jugador,190,30,rojo);
											juego->actualizarPantalla();
											juego->dibujarCartaJugador(jugador_aux);
										}else{
											juego->pantalla->escribirTextoDesdePos("El jugador no posee cartas",5,juego->infoconfig->alto*(0.95),24,rojo);
											juego->actualizarPantalla();
											sleep(1);
										}
									}
								}
							}
						}
					}else{
						juego->pantalla->escribirTextoDesdePos("No es un usuario 'Observador'",5,juego->infoconfig->alto*(0.95),24,rojo);
						juego->actualizarPantalla();
						sleep(1);
					}
				}
			}
		}
	}
}
Juego::Juego() {
	this->parser=new ParserCliente(PATHARCHIVOCONF);
	this->parserResultado=new ParserResultadoCliente();
	//ParserCliente *parserAux=new ParserCliente(PATHARCHIVOCONF);

	if(this->parser->comprobarSintaxis()){

		this->infoconfig = this->parser->getInformacionConfig();
		cout << "alto y ancho" << endl;
		cout << this->infoconfig->alto << endl;
		cout << this->infoconfig->ancho << endl;
		if(!this->verificarResolucion(this->infoconfig->alto,this->infoconfig->ancho)){
			this->informarError("G","V","Resolucion invalida en el archivo config.ini");
			exit(0);
		}
		cout << "puerto e ip" << endl;
		cout << this->infoconfig->puerto << endl;
		cout << this->infoconfig->ip << endl;
//		if(this->infoconfig->puerto != 54340){
//			this->informarError("G","V","Puerto invalido en el archivo config.ini, tiene que ser el 54340");
//			exit(0);
//		}
		this->cliente=new Cliente(this->infoconfig->puerto, this->infoconfig->ip);
		//this->cliente = new Cliente();
		if(!(cliente->conectar())){
			this->informarError("G","V","Puerto y/o ip invalido en el archivo config.ini");
			cout<<"Se cerrara la apliacion..."<<endl;
			sleep(1);
			exit(0);
		}
		this->pantalla = new Pantalla(this->infoconfig->alto, this->infoconfig->ancho);
	}
	else {
		cout<<"Error de Sintaxis en el archivo config.ini"<<endl;
		cout<<"Se cerrara la aplicacion..."<<endl;
		sleep(2);
		exit(0);
	}
	//delete parserAux;
}

bool Juego::verificarResolucion(unsigned int alto,unsigned  int ancho){
	if((alto==480&&ancho==640) || (alto==600 && ancho==800) || (alto==768 && ancho==1024) || (alto==768 && ancho==1280))
		return true;
	else
		return false;

}

void Juego::informarError(string idOperacion, string tipoError, string mensaje){
	list<string>* mensajesError = new list<string> ();
	mensajesError->push_back(tipoError);
	mensajesError->push_back(mensaje);
	this->parser->registrarError(idOperacion, mensajesError);
	cout << mensaje << endl;
}

Juego::~Juego() {
	delete pantalla;
	delete cliente;
	delete parser;
	delete parserResultado;
	delete infoconfig;
}

string Juego::pedirEscenario(){
	string ruta=PATHESCENARIO;
	//Pido el escenario
	string idOperacion="E";
	list<string>* operandos=new list<string>();
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	delete operandos;
	cliente->enviar(xml);
	cliente->recibirArchivo(ruta);
	return ruta;
}

string Juego::pedirImagenJugador(Jugador * jugador){
	string ruta=jugador->getNombre() + ".bmp";
	jugador->setPath(ruta);
	//Pido la imagen del jugador
	string idOperacion="I";
	list<string>* operandos=new list<string>();
	list<string>::iterator it=operandos->begin();
	it=operandos->insert(it,"jugador");
	it++;
	it=operandos->insert(it,jugador->getNombre());
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	delete operandos;
	cliente->enviar(xml);
	cliente->recibirArchivo(ruta);
	return ruta;
}

bool Juego::enviarImagenJugador(string ruta,string jugador){
	//Pido la imagen del jugador
	ruta+=".bmp";
	string idOperacion="Z";
	list<string>* operandos=new list<string>();
	list<string>::iterator it=operandos->begin();
	it=operandos->insert(it,"jugador");
	it++;
	it=operandos->insert(it,jugador);
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	delete operandos;
	cliente->enviar(xml);
	char* respuesta=cliente->recibirRespuesta();
	cliente->enviarArchivoBMP(ruta);
	respuesta=cliente->recibirRespuesta();
	return true;
}

void Juego::dibujarEscenario(string path){
	BitMap* escenario=new BitMap(path);
	if((escenario->esUnaImagenCorrecta())and(escenario->getAlto()>1)and(escenario->getAncho()>1)){
		escenario->resizeTo(this->infoconfig->alto, this->infoconfig->ancho);
		pantalla->dibujarBitMapDesdePos((*escenario),0,0);
	}else{

		this->informarError("B","E","El escenario no es una imagen BMP o esta corrupta");
	}
}


void Juego::dibujarJugador(Jugador jugadorADibujar){
	BitMap* jugador = new BitMap(jugadorADibujar.getPath());
	SDL_Color color;
	color.r=240;
	color.b=55;
	color.g=200;
	//dibujaria con el sdl_ttf el nombre y la plata al lado de la imagen
	if ((jugador->esUnaImagenCorrecta())and(jugador->getAlto()>1)and(jugador->getAncho()>1)) {
		int tamImagen = this->infoconfig->ancho / 10;
		jugador->resizeTo(tamImagen, tamImagen);
		const char* nombre = jugadorADibujar.getNombre().c_str();
		const char* plata = jugadorADibujar.getPlata().c_str();
		int tamfuente = this->infoconfig->ancho / 32.5;
		int factor = this->infoconfig->alto / 110;
		if (jugadorADibujar.getId() == 1) {
			pantalla->dibujarBitMapDesdePosCircular(*jugador,
					this->infoconfig->ancho / 8, this->infoconfig->alto / 2);
			pantalla->escribirTextoDesdePos(nombre, (this->infoconfig->ancho / 8)+(tamImagen/4), (this->infoconfig->alto / 2)+tamImagen+factor, tamfuente, color );
			pantalla->escribirTextoDesdePos(plata, (this->infoconfig->ancho / 8)+(tamImagen/4), (this->infoconfig->alto / 2)+tamImagen+factor+tamfuente, tamfuente,color );
		} else if (jugadorADibujar.getId() == 2) {
			pantalla->dibujarBitMapDesdePosCircular(*jugador,
					this->infoconfig->ancho / 5.4, this->infoconfig->alto / 3.2);
			pantalla->escribirTextoDesdePos(nombre, (this->infoconfig->ancho / 5.4)+(tamImagen/4), (this->infoconfig->alto / 3.2)-(2*tamfuente)-factor, tamfuente ,color);
			pantalla->escribirTextoDesdePos(plata, (this->infoconfig->ancho / 5.4)+(tamImagen/4), (this->infoconfig->alto / 3.2)-(tamfuente)-factor, tamfuente,color );
		} else if (jugadorADibujar.getId() == 3) {
			pantalla->dibujarBitMapDesdePosCircular(*jugador,
					this->infoconfig->ancho / 2.8, this->infoconfig->alto / 3.9);
			pantalla->escribirTextoDesdePos(nombre, (this->infoconfig->ancho / 2.8)+(tamImagen/4), (this->infoconfig->alto / 3.9)-(2*tamfuente)-factor, tamfuente ,color);
			pantalla->escribirTextoDesdePos(plata, (this->infoconfig->ancho / 2.8)+(tamImagen/4), (this->infoconfig->alto / 3.9)-(tamfuente)-factor, tamfuente ,color);
		} else if (jugadorADibujar.getId() == 4) {
			pantalla->dibujarBitMapDesdePosCircular(*jugador,
					this->infoconfig->ancho / 1.8, this->infoconfig->alto / 3.9);
			pantalla->escribirTextoDesdePos(nombre, (this->infoconfig->ancho / 1.8)+(tamImagen/4), (this->infoconfig->alto / 3.9)-(2*tamfuente)-factor, tamfuente,color );
			pantalla->escribirTextoDesdePos(plata, (this->infoconfig->ancho / 1.8)+(tamImagen/4), (this->infoconfig->alto / 3.9)-(tamfuente)-factor, tamfuente,color );
		} else if (jugadorADibujar.getId() == 5) {
			pantalla->dibujarBitMapDesdePosCircular(*jugador,
					this->infoconfig->ancho / 1.4, this->infoconfig->alto / 3.2);
			pantalla->escribirTextoDesdePos(nombre, (this->infoconfig->ancho / 1.4)+(tamImagen/4), (this->infoconfig->alto / 3.2)-(2*tamfuente)-factor, tamfuente,color );
			pantalla->escribirTextoDesdePos(plata, (this->infoconfig->ancho / 1.4)+(tamImagen/4), (this->infoconfig->alto / 3.2)-(tamfuente)-factor, tamfuente,color );
		} else if (jugadorADibujar.getId() == 6) {
			pantalla->dibujarBitMapDesdePosCircular(*jugador,
					this->infoconfig->ancho / 1.3, this->infoconfig->alto / 2);
			pantalla->escribirTextoDesdePos(nombre, (this->infoconfig->ancho / 1.3)+(tamImagen/4), (this->infoconfig->alto / 2)+tamImagen+factor, tamfuente,color );
			pantalla->escribirTextoDesdePos(plata, (this->infoconfig->ancho / 1.3)+(tamImagen/4), (this->infoconfig->alto / 2)+tamImagen+factor+tamfuente, tamfuente,color );
		}
	} else {
		this->informarError("B","E","El jugador " + jugadorADibujar.getNombre()+ " no es una imagen BMP o esta corrupta");
			}
}


list<Jugador>* Juego::pedirJugadores(){
	string idOperacion="J";
	list<string>* operandos=new list<string>();
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	delete operandos;
	cliente->enviar(xml);
	char * respuesta=cliente->recibirRespuesta();
	list<string>* jugadoresAux=parserResultado->getJugadores(respuesta);
	list<string>::iterator it;
	list <Jugador> * jugadores=new list<Jugador>();
	list<Jugador>::iterator iterador=jugadores->begin();
	it = jugadoresAux->begin();
	for(unsigned int i = 0; i < jugadoresAux->size()/2; i++){
		string nombre=(*it);
		it++;
		string plata=(*it);
		it++;
		iterador=jugadores->insert(iterador,Jugador("",nombre,plata,i+1));
		iterador++;
	}
	this->jugadores = jugadores;
	return jugadores;
}

void Juego::dibujarCarta(Carta cartaADibujar){
	BitMap* carta = new BitMap("Cartas/" + cartaADibujar.getPalo()+"-"+cartaADibujar.getNumero()+".bmp");
	//BitMap* carta = new BitMap("quilmes.bmp");
	if ((carta->esUnaImagenCorrecta())and(carta->getAlto()>1)and(carta->getAncho()>1)) {
		int tamImagen = this->infoconfig->ancho / 22;
		carta->resizeTo(2.5 * tamImagen,2*tamImagen);
		int id = cartaADibujar.getId();
		if (id == 1) {
			pantalla->dibujarBitMapDesdePos(*carta,
					this->infoconfig->ancho / 3.15, (this->infoconfig->alto / 2.3));
		} else if (id == 2) {
			pantalla->dibujarBitMapDesdePos(*carta,
					this->infoconfig->ancho / 2.58, (this->infoconfig->alto / 2.3));
		} else if (id == 3) {
			pantalla->dibujarBitMapDesdePos(*carta,
					this->infoconfig->ancho / 2.2, (this->infoconfig->alto / 2.3));
		} else if (id == 4) {
			pantalla->dibujarBitMapDesdePos(*carta,
					this->infoconfig->ancho / 1.88, (this->infoconfig->alto / 2.3));
		} else if (id == 5) {
			pantalla->dibujarBitMapDesdePos(*carta,
					this->infoconfig->ancho / 1.66, (this->infoconfig->alto / 2.3));
		}
	}else {
		this->informarError("B","E","La carta " + cartaADibujar.getNumero() + " de " + cartaADibujar.getPalo() +	" no tiene un imagen BMP o esta corrupta");
		}

}

list<Carta>* Juego::pedirCartas(){
	string idOperacion="C";
	list<string>* operandos=new list<string>();
	char* xml = parser->getXmlDeOperacion(idOperacion, operandos);
	cliente->enviar(xml);
	char * respuesta = cliente->recibirRespuesta();
	list<string>* cartasAux = parserResultado->getCartas(respuesta);
	list<string>::iterator it;
	list<Carta> * cartas = new list<Carta> ();
	list<Carta>::iterator iterador = cartas->begin();
	it = cartasAux->begin();
	for (unsigned int i = 0; i < cartasAux->size() / 2; i++) {
		string palo = (*it);
		it++;
		string numero = (*it);
		it++;
		iterador = cartas->insert(iterador,
				Carta("", palo, numero, i + 1));
		iterador++;
	}
	this->cartas = cartas;
	return cartas;
}

list<Carta>* Juego::pedirCartasJugador(Jugador * jugador){
	string idOperacion="B";
	list<string>* operandos=new list<string>();
	list<string>::iterator it_=operandos->begin();
	it_=operandos->insert(it_,"usuario");
	it_++;
	it_=operandos->insert(it_,jugador->getNombre());
	char* xml = parser->getXmlDeOperacion(idOperacion, operandos);
	cliente->enviar(xml);
	char * respuesta = cliente->recibirRespuesta();
	list<string>* cartasAux = parserResultado->getCartas(respuesta);
	list<string>::iterator it;
	list<Carta> * cartas = new list<Carta> ();
	if(cartasAux!=NULL){
		list<Carta>::iterator iterador = cartas->begin();
		it = cartasAux->begin();
		for (unsigned int i = 0; i < cartasAux->size() / 2; i++) {
			string palo = (*it);
			it++;
			string numero = (*it);
			it++;
			iterador = cartas->insert(iterador,
					Carta("", palo, numero, i + 1));
			iterador++;
		}
		jugador->setCartas(cartas);
	}
	return cartas;
}
void Juego::dibujarCartaJugador(Jugador * jugador){
	SDL_Color blanco;
	blanco.r=255;
	blanco.g=255;
	blanco.b=255;
//	this->pantalla->escribirTextoDesdePos("Cartas de:",this->infoconfig->ancho/2 - 90,2,30,blanco);
//	this->pantalla->escribirTextoDesdePos(jugador->getNombre().c_str(),this->infoconfig->alto/2 + 110,2,30,blanco);
	Carta *carta1,*carta2;
	carta1=&jugador->getCartas()->front();
	carta2=&jugador->getCartas()->back();
	BitMap* imagen_carta1 = new BitMap("Cartas/" + carta1->getPalo()+"-"+carta1->getNumero()+".bmp");
	BitMap* imagen_carta2 = new BitMap("Cartas/" + carta2->getPalo()+"-"+carta2->getNumero()+".bmp");
	if ((imagen_carta1->esUnaImagenCorrecta())and(imagen_carta1->getAlto()>1)and(imagen_carta1->getAncho()>1)) {
		imagen_carta1->resizeTo(this->infoconfig->alto/5,this->infoconfig->ancho/5);
		this->pantalla->dibujarBitMapDesdePos(*imagen_carta1,this->infoconfig->ancho/2 - 70,this->infoconfig->alto/2 + 60);
	}
	if ((imagen_carta2->esUnaImagenCorrecta())and(imagen_carta2->getAlto()>1)and(imagen_carta2->getAncho()>1)) {
		imagen_carta2->resizeTo(this->infoconfig->alto/5,this->infoconfig->ancho/5);
		this->pantalla->dibujarBitMapDesdePos(*imagen_carta2,this->infoconfig->ancho/2 - 20,this->infoconfig->alto/2 + 60);
	}
	this->actualizarPantalla();
	sleep(1);

}

bool Juego::validarJugador(string usuario, string pass){
	string idOperacion="U";
	list<string>* operandos=new list<string>();
	list<string>::iterator it=operandos->begin();
	it=operandos->insert(it,"usuario");
	it++;
	it=operandos->insert(it,usuario);
	it++;
	it=operandos->insert(it,"password");
	it++;
	it=operandos->insert(it,pass);
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	delete operandos;
	cliente->enviar(xml);
	char * respuesta = cliente->recibirRespuesta();
	return (this->parserResultado->DecodificaResultado(respuesta));
}

bool Juego::registrarJugador(string usuario, string pass){
	string idOperacion="R";
	list<string>* operandos=new list<string>();
	list<string>::iterator it=operandos->begin();
		it=operandos->insert(it,"usuario");
		it++;
		it=operandos->insert(it,usuario);
		it++;
		it=operandos->insert(it,"password");
		it++;
		it=operandos->insert(it,pass);
		char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
		delete operandos;
		cliente->enviar(xml);
		char * respuesta = cliente->recibirRespuesta();
		if(this->parserResultado->DecodificaResultado(respuesta)){
			string ruta("boton");
			string jugador(usuario);
			if(enviarImagenJugador(ruta,jugador)){
				cout<<"Archivo enviado"<<endl;
			}
			return true;
		}
		return false;
}

void Juego::dibujarBoton(string textoBoton, int pos){
	BitMap* boton = new BitMap("boton.bmp");
	SDL_Color color;
	color.r=255;
	color.g=255;
	color.b=255;
	if ((boton->esUnaImagenCorrecta())and(boton->getAlto()>1)and(boton->getAncho()>1)) {
		int tamImagenancho = this->infoconfig->ancho / 7.0;
		int tamImagenalto = this->infoconfig->alto / 5;
		int tamfuente = this->infoconfig->ancho / 42;
		int factor = this->infoconfig->ancho / 100;
		const char* texto = textoBoton.c_str();
		boton->resizeTo(tamImagenalto, tamImagenancho);
		if (pos == 1) {
			pantalla->dibujarBitMapDesdePosCircular(*boton, this->infoconfig->ancho / 2, (this->infoconfig->alto / 1.3));
			pantalla->escribirTextoDesdePos(texto,(this->infoconfig->ancho / 2)+factor ,(this->infoconfig->alto / 1.3)+(tamImagenalto/2)-(tamfuente/2), tamfuente,color);
		} else if (pos == 2) {
			pantalla->dibujarBitMapDesdePosCircular(*boton, this->infoconfig->ancho / 1.5, (this->infoconfig->alto / 1.3));
			pantalla->escribirTextoDesdePos(texto,(this->infoconfig->ancho / 1.5)+factor ,(this->infoconfig->alto / 1.3)+(tamImagenalto/2)-(tamfuente/2), tamfuente,color);
		} else if (pos == 3) {
			pantalla->dibujarBitMapDesdePosCircular(*boton, this->infoconfig->ancho / 1.18, (this->infoconfig->alto / 1.3));
			pantalla->escribirTextoDesdePos(texto,(this->infoconfig->ancho / 1.18)+factor ,(this->infoconfig->alto / 1.3)+(tamImagenalto/2)-(tamfuente/2), tamfuente,color);
		}
	} else {
		this->informarError("B","E","Uno de los botones no es una imagen BMP o esta corrupta");
		}

}

void Juego::dibujarPoso(){
	int tamfuente = this->infoconfig->ancho / 28;
	string pozo = "BOTE $: " + this->posoAcumulado;
	const char* bote = pozo.c_str();
	SDL_Color color;
	color.r=255;
	color.g=255;
	color.b=255;
	pantalla->escribirTextoDesdePos(bote, this->infoconfig->ancho / 2.5 ,(this->infoconfig->alto / 56) , tamfuente,color);

}

void Juego::pedirPoso(){
	string idOperacion="P";
	list<string>* operandos=new list<string>();
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	cliente->enviar(xml);
	char * respuesta=cliente->recibirRespuesta();
	posoAcumulado=parserResultado->getPoso(respuesta);
}

void Juego::actualizarPantalla(){
	this->pantalla->actualizarPantalla(0,0,0,0);
}

void Juego::dibujarPantallaPrincipal(){
	this->pantalla->dibujarRectangulo(0,0,0,0,255,255,255);
	SDL_Color blanco;
	blanco.r=255;
	blanco.g=255;
	blanco.b=255;
	this->dibujarEscenario("boton.bmp");
	this->pantalla->escribirTextoDesdePos("Loguearse",10,10,40,blanco);
	this->pantalla->escribirTextoDesdePos("Registrarse",10,100,40,blanco);
	this->pantalla->escribirTextoDesdePos("Observar",10,190,40,blanco);
	this->actualizarPantalla();
	bool terminar=false;
	SDL_Event evento;
	while(!terminar){
		if(SDL_PollEvent(&evento)) {
			//Es importante probar el quit primero porque tambien es un evento de mouse o teclado
			if(evento.type == SDL_QUIT){
				exit(0);
			} else if(evento.type == SDL_MOUSEBUTTONDOWN){
				if(evento.button.button==1){
					if(evento.button.x>=5 and evento.button.x<=155){
						if(evento.button.y>=10 and evento.button.y<=65){
							this->dibujarPantallaLogin(false,0,false);
							terminar=true;
						}
						else if(evento.button.y>=100 and evento.button.y<=155){
							this->dibujarPantallaRegistro(0);
							terminar=true;
						}
						else if(evento.button.y>=190 and evento.button.y<=245){
//							this->dibujarPantallaLogin(false,0,true);
							this->jugar(true,false);
							terminar=true;
						}
					}
				}
			}
		}
	}
}

void Juego::dibujarPantallaLogin(bool usuarioIncorrecto, int cantidadIntentos, bool jugador_observador){
	this->dibujarEscenario("boton.bmp");
	SDL_Color blanco;
	blanco.r=255;
	blanco.g=255;
	blanco.b=255;
	SDL_Color negro;
	negro.r=0;
	negro.g=0;
	negro.b=0;
	SDL_Color rojo;
	rojo.r=255;
	rojo.g=0;
	rojo.b=0;
	//Para informar errores
	this->pantalla->dibujarRectangulo(0,this->infoconfig->alto*(0.9),this->infoconfig->ancho,24,255,255,255);
	if(cantidadIntentos>2){
		this->pantalla->escribirTextoDesdePos("Se Superaron los 3 intentos se cerrara la aplicacion",5,this->infoconfig->alto*(0.9),24,rojo);
		this->actualizarPantalla();
		sleep(5);
		exit(0);
	}
	if(usuarioIncorrecto){
		this->pantalla->escribirTextoDesdePos("Usuario Incorrecto",5,this->infoconfig->alto*(0.9),24,rojo);
		this->actualizarPantalla();
	}
	this->pantalla->dibujarRectangulo(8,55,144,20,255,255,255);
	this->pantalla->dibujarRectangulo(8,165,144,20,255,255,255);
	this->pantalla->escribirTextoDesdePos("Usuario",10,10,40,blanco);
	this->pantalla->escribirTextoDesdePos("Contrasena",10,120,40,blanco);
	string usuarioTexto("");
	string contrasenaTexto("");
	string contrasenaAsteriscos("");
	CasilleroTexto casillero=NINGUNO;
	this->pantalla->escribirStringDesdePos(&usuarioTexto,13,50,25,0,0,0);
	this->pantalla->escribirStringDesdePos(&contrasenaAsteriscos,13,160,25,0,0,0);
	this->actualizarPantalla();
	char caracterLeido=(char)0;
	bool terminar=false;
	SDL_Event evento;
	while(!terminar){
		if(SDL_PollEvent(&evento)) {
			//Es importante probar el quit primero porque tambien es un evento de mouse o teclado
			if(evento.type == SDL_QUIT){
				exit(0);
			} else if(evento.type == SDL_MOUSEBUTTONDOWN){
				if(evento.button.button==1){
					if(evento.button.x>=8 and evento.button.x<=152){
						if(evento.button.y>=55 and evento.button.y<=75){
							this->pantalla->dibujarRectangulo(8,55,144,20,200,200,200);
							this->pantalla->dibujarRectangulo(8,165,144,20,255,255,255);
							this->pantalla->escribirStringDesdePos(&usuarioTexto,13,50,25,0,0,0);
							this->pantalla->escribirStringDesdePos(&contrasenaAsteriscos,13,160,25,0,0,0);
							casillero=USUARIO;
							this->actualizarPantalla();
						}
						else if(evento.button.y>=165 and evento.button.y<=185){
							this->pantalla->dibujarRectangulo(8,55,144,20,255,255,255);
							this->pantalla->dibujarRectangulo(8,165,144,20,200,200,200);
							this->pantalla->escribirStringDesdePos(&usuarioTexto,13,50,25,0,0,0);
							this->pantalla->escribirStringDesdePos(&contrasenaAsteriscos,13,160,25,0,0,0);
							casillero=CONTRASENA;
							this->actualizarPantalla();
						}
					}
				}
			} else if(evento.type == SDL_KEYDOWN){
				if(evento.key.keysym.sym==SDLK_BACKSPACE){
					if(casillero==USUARIO){
						int nuevoTamanio=usuarioTexto.size()-1;
						if (nuevoTamanio>=0){
							usuarioTexto.resize(usuarioTexto.size()-1);
							this->pantalla->dibujarRectangulo(8,55,144,20,200,200,200);
							this->pantalla->escribirStringDesdePos(&usuarioTexto,13,50,25,0,0,0);
							this->actualizarPantalla();
						}
					} else if(casillero==CONTRASENA){
						int nuevoTamanio=contrasenaTexto.size()-1;
						if (nuevoTamanio>=0){
							contrasenaTexto.resize(contrasenaTexto.size()-1);
							contrasenaAsteriscos.resize(contrasenaAsteriscos.size()-1);
							this->pantalla->dibujarRectangulo(8,165,144,20,200,200,200);
							this->pantalla->escribirStringDesdePos(&contrasenaAsteriscos,13,160,25,0,0,0);
							this->actualizarPantalla();
						}
					}
				} else if(evento.key.keysym.sym==SDLK_RETURN){
					if(!this->validarJugador(usuarioTexto,contrasenaTexto)){
						this->dibujarPantallaLogin(true,cantidadIntentos + 1,jugador_observador);
					}else{
						this->nombreJugador=usuarioTexto;
						this->pantalla->dibujarRectangulo(0,this->infoconfig->alto*(0.9),this->infoconfig->ancho,24,255,255,255);
						this->pantalla->escribirTextoDesdePos("Se logueo con exito",5,this->infoconfig->alto*(0.9),24,rojo);
						this->actualizarPantalla();
						sleep(2);
						this->jugar(jugador_observador,false);
					}
					//entre la a y la z o entre el 0 y el 9
				} else if(((evento.key.keysym.sym>=97) and (evento.key.keysym.sym<=122) )or((evento.key.keysym.sym>=48) and (evento.key.keysym.sym<=57) )){
					caracterLeido=(char)evento.key.keysym.unicode;
					if(casillero==USUARIO){
						usuarioTexto+=caracterLeido;
//						cout<<"caracter leido/"<<caracterLeido<<"/"<<endl;
//						cout<<usuarioTexto<<endl;
						this->pantalla->dibujarRectangulo(8,55,144,20,200,200,200);
						this->pantalla->escribirStringDesdePos(&usuarioTexto,13,50,25,0,0,0);
						this->actualizarPantalla();
					}
					if(casillero==CONTRASENA){
						contrasenaTexto+=caracterLeido;
						contrasenaAsteriscos+="*";
						this->pantalla->dibujarRectangulo(8,165,144,20,200,200,200);
						this->pantalla->escribirStringDesdePos(&contrasenaAsteriscos,13,160,25,0,0,0);
						this->actualizarPantalla();
					}
				}
			}
		}
	}
}

void Juego::dibujarPantallaRegistro(int cantidadIntentos){
//	this->pantalla->dibujarRectangulo(0,0,0,0,255,255,255);
	this->dibujarEscenario("boton.bmp");
	SDL_Color blanco;
	blanco.r=255;
	blanco.g=255;
	blanco.b=255;
	SDL_Color negro;
	negro.r=0;
	negro.g=0;
	negro.b=0;
	SDL_Color rojo;
	rojo.r=255;
	rojo.g=0;
	rojo.b=0;
	//Para informar errores
	this->pantalla->dibujarRectangulo(0,this->infoconfig->alto*(0.9),this->infoconfig->ancho,24,255,255,255);
	if(cantidadIntentos>2){
		this->pantalla->escribirTextoDesdePos("Se Superaron los 3 intentos se cerrara la aplicacion",5,this->infoconfig->alto*(0.9),24,rojo);
		this->actualizarPantalla();
		sleep(5);
		exit(0);
	}
	if(cantidadIntentos>=1){
		this->pantalla->escribirTextoDesdePos("El usuario ya existe",5,this->infoconfig->alto*(0.9),24,rojo);
		this->actualizarPantalla();
	}
	this->pantalla->dibujarRectangulo(8,55,144,20,255,255,255);
	this->pantalla->dibujarRectangulo(8,165,144,20,255,255,255);
	this->pantalla->dibujarRectangulo(8,275,144,20,255,255,255);
	this->pantalla->escribirTextoDesdePos("Usuario",10,10,40,blanco);
	this->pantalla->escribirTextoDesdePos("Contrasena",10,120,40,blanco);
	this->pantalla->escribirTextoDesdePos("Archivo",10,230,40,blanco);
	string usuarioTexto("");
	string contrasenaTexto("");
	string contrasenaAsteriscos("");
	string archivoTexto("");
	CasilleroTexto casillero=NINGUNO;
	this->pantalla->escribirStringDesdePos(&usuarioTexto,13,50,25,0,0,0);
	this->pantalla->escribirStringDesdePos(&contrasenaAsteriscos,13,160,25,0,0,0);
	this->pantalla->escribirStringDesdePos(&archivoTexto,13,270,25,0,0,0);
	this->actualizarPantalla();
	char caracterLeido=(char)0;
	bool terminar=false;
	SDL_Event evento;
	while(!terminar){
		if(SDL_PollEvent(&evento)) {
			//Es importante probar el quit primero porque tambien es un evento de mouse o teclado
			if(evento.type == SDL_QUIT){
				exit(0);
			} else if(evento.type == SDL_MOUSEBUTTONDOWN){
				if(evento.button.button==1){
					if(evento.button.x>=8 and evento.button.x<=152){
						if(evento.button.y>=55 and evento.button.y<=75){
							this->pantalla->dibujarRectangulo(8,55,144,20,200,200,200);
							this->pantalla->dibujarRectangulo(8,165,144,20,255,255,255);
							this->pantalla->dibujarRectangulo(8,275,144,20,255,255,255);
							this->pantalla->escribirStringDesdePos(&usuarioTexto,13,50,25,0,0,0);
							this->pantalla->escribirStringDesdePos(&contrasenaAsteriscos,13,160,25,0,0,0);
							this->pantalla->escribirStringDesdePos(&archivoTexto,13,270,25,0,0,0);
							casillero=USUARIO;
							this->actualizarPantalla();
						}
						else if(evento.button.y>=165 and evento.button.y<=185){
							this->pantalla->dibujarRectangulo(8,55,144,20,255,255,255);
							this->pantalla->dibujarRectangulo(8,165,144,20,200,200,200);
							this->pantalla->dibujarRectangulo(8,275,144,20,255,255,255);
							this->pantalla->escribirStringDesdePos(&usuarioTexto,13,50,25,0,0,0);
							this->pantalla->escribirStringDesdePos(&contrasenaAsteriscos,13,160,25,0,0,0);
							this->pantalla->escribirStringDesdePos(&archivoTexto,13,270,25,0,0,0);
							casillero=CONTRASENA;
							this->actualizarPantalla();
						}
						else if(evento.button.y>=275 and evento.button.y<=295){
							this->pantalla->dibujarRectangulo(8,55,144,20,255,255,255);
							this->pantalla->dibujarRectangulo(8,165,144,20,255,255,255);
							this->pantalla->dibujarRectangulo(8,275,144,20,200,200,200);
							this->pantalla->escribirStringDesdePos(&usuarioTexto,13,50,25,0,0,0);
							this->pantalla->escribirStringDesdePos(&contrasenaAsteriscos,13,160,25,0,0,0);
							this->pantalla->escribirStringDesdePos(&archivoTexto,13,270,25,0,0,0);
							casillero=ARCHIVO;
							this->actualizarPantalla();
						}
					}
				}
			} else if(evento.type == SDL_KEYDOWN){
				if(evento.key.keysym.sym==SDLK_BACKSPACE){
					if(casillero==USUARIO){
						int nuevoTamanio=usuarioTexto.size()-1;
						if (nuevoTamanio>=0){
							usuarioTexto.resize(usuarioTexto.size()-1);
							this->pantalla->dibujarRectangulo(8,55,144,20,200,200,200);
							this->pantalla->escribirStringDesdePos(&usuarioTexto,13,50,25,0,0,0);
							this->actualizarPantalla();
						}
					} else if(casillero==CONTRASENA){
						int nuevoTamanio=contrasenaTexto.size()-1;
						if (nuevoTamanio>=0){
							contrasenaTexto.resize(contrasenaTexto.size()-1);
							contrasenaAsteriscos.resize(contrasenaAsteriscos.size()-1);
							this->pantalla->dibujarRectangulo(8,165,144,20,200,200,200);
							this->pantalla->escribirStringDesdePos(&contrasenaAsteriscos,13,160,25,0,0,0);
							this->actualizarPantalla();
						}
					} else if(casillero==ARCHIVO){
						int nuevoTamanio=archivoTexto.size()-1;
						if (nuevoTamanio>=0){
							archivoTexto.resize(archivoTexto.size()-1);
							this->pantalla->dibujarRectangulo(8,275,144,20,200,200,200);
							this->pantalla->escribirStringDesdePos(&archivoTexto,13,270,25,0,0,0);
							this->actualizarPantalla();
						}
					}
				} else if(evento.key.keysym.sym==SDLK_RETURN){
					if(!this->registrarJugador(usuarioTexto,contrasenaTexto)){
						this->dibujarPantallaRegistro(cantidadIntentos + 1);
					}else{
						this->nombreJugador=usuarioTexto;
						this->pantalla->dibujarRectangulo(0,this->infoconfig->alto*(0.9),this->infoconfig->ancho,24,255,255,255);
						this->pantalla->escribirTextoDesdePos("Se registro el usuario",5,this->infoconfig->alto*(0.9),24,rojo);
						this->actualizarPantalla();
						sleep(2);
						this->dibujarPantallaPrincipal();
					}
					//entre la a y la z o entre el 0 y el 9
				} else if(((evento.key.keysym.sym>=97) and (evento.key.keysym.sym<=122) )or((evento.key.keysym.sym>=48) and (evento.key.keysym.sym<=57) )){
					caracterLeido=(char)evento.key.keysym.unicode;
					if(casillero==USUARIO){
						usuarioTexto+=caracterLeido;
						this->pantalla->dibujarRectangulo(8,55,144,20,200,200,200);
						this->pantalla->escribirStringDesdePos(&usuarioTexto,13,50,25,0,0,0);
						this->actualizarPantalla();
					}
					if(casillero==CONTRASENA){
						contrasenaTexto+=caracterLeido;
						contrasenaAsteriscos+="*";
						this->pantalla->dibujarRectangulo(8,165,144,20,200,200,200);
						this->pantalla->escribirStringDesdePos(&contrasenaAsteriscos,13,160,25,0,0,0);
						this->actualizarPantalla();
					}
					if(casillero==ARCHIVO){
						archivoTexto+=caracterLeido;
						this->pantalla->dibujarRectangulo(8,275,144,20,200,200,200);
						this->pantalla->escribirStringDesdePos(&archivoTexto,13,270,25,0,0,0);
						this->actualizarPantalla();
					}
				}
			}
		}
	}
}

void Juego::empezarPartida(){
	string idOperacion="F";
	list<string>* operandos=new list<string>();
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	cliente->enviar(xml);
}

bool Juego::esMiTurno(){
	string idOperacion="A";
	list<string>* operandos=new list<string>();
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	cliente->enviar(xml);
	char * respuesta=cliente->recibirRespuesta();
	string jugadorTurno=parserResultado->getPoso(respuesta);
	return jugadorTurno==this->nombreJugador;
}

list<Jugador> Juego::getJugadores(){
	if(this->jugadores!=NULL)
		return *this->jugadores;
	list<Jugador> lista;
	return lista;
}

void Juego::jugar(bool jugador_observador, bool jugador_virtual){
	string path,pathEscenario;
	if(this->enviarImagenJugador("boton","gaston")) cout<<"envio imagen"<<endl;
	SDL_Color rojo;
	rojo.r=255;
	rojo.g=0;
	rojo.b=0;
	SDL_Color blanco;
	blanco.r=255;
	blanco.g=255;
	blanco.b=255;
	pthread_t thread;
	int create=pthread_create(&thread,NULL,manejoEventos,(void*)this);
	while(true){
		pathEscenario = this->pedirEscenario();
		list<Carta>* cartas = this->pedirCartas();
		list<Jugador>* jugadores = this->pedirJugadores();
		this->pedirPoso();
		this->dibujarEscenario(pathEscenario);
		list<Jugador>::iterator it = jugadores->begin();
		int x_nombre_jugador=5, y_nombre_jugador=5;
		y_nombre_jugador+=35;
		list<Carta>::iterator it2 = cartas->begin();
		while (it2 != cartas->end()) {
			this->dibujarCarta(*it2);
			it2++;
		}
		while (it != jugadores->end()) {
			path = this->pedirImagenJugador(&(*it));
			this->pedirCartasJugador(&(*it));
			this->dibujarJugador(*it);
			this->pantalla->escribirTextoDesdePos((*it).getNombre().c_str(),x_nombre_jugador,y_nombre_jugador,30,blanco);
			y_nombre_jugador+=30;
			it++;
		}
		if(jugadores->size()<6){
			for(int i=(jugadores->size() + 1);i<7;i++){
				Jugador jugador("ImagenVacio.bmp"," "," ",i);
				this->dibujarJugador(jugador);
			}
		}
		if(cartas->size()<5){
			for(int i=(cartas->size()+1);i<6;i++){
				Carta carta("Imagen-Carta.bmp","Imagen","Carta",i);
				this->dibujarCarta(carta);
			}
		}
		this->dibujarPoso();
		this->pantalla->dibujarRectangulo(0,this->infoconfig->alto*(0.95),this->infoconfig->ancho,24,255,255,255);
		this->pantalla->escribirTextoDesdePos("Pedir cartas de:",5,8,40,blanco);
		this->actualizarPantalla();
		sleep(5);
	}
}

Jugador * Juego::getJugador(int id){
	list<Jugador>::iterator it = jugadores->begin();
	while (it != jugadores->end()) {
		if((*it).getId()==id) return &(*it);
		it++;
	}
	return NULL;
}
