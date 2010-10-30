#include "Juego.h"

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
	this->pantalla->dibujarRectangulo(5,10,150,55,0,0,0);
	this->pantalla->dibujarRectangulo(5,100,150,55,0,0,0);
	this->pantalla->dibujarRectangulo(5,190,150,55,0,0,0);
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
							this->dibujarPantallaLogin(false,0);
							terminar=true;
						}
						else if(evento.button.y>=100 and evento.button.y<=155){
							this->pantalla->dibujarRectangulo(5,100,150,55,0,0,0);
							this->actualizarPantalla();
							terminar=true;
						}
						else if(evento.button.y>=190 and evento.button.y<=245){
							this->pantalla->dibujarRectangulo(5,190,150,55,0,0,0);
							this->actualizarPantalla();
							terminar=true;
						}
					}
				}
			}
		}
	}
}

void Juego::dibujarPantallaLogin(bool usuarioIncorrecto, int cantidadIntentos){
	this->pantalla->dibujarRectangulo(0,0,0,0,255,255,255);
		SDL_Color blanco;
		blanco.r=255;
		blanco.g=255;
		blanco.b=255;
		SDL_Color negro;
		negro.r=0;
		negro.g=0;
		negro.b=0;
		this->pantalla->dibujarRectangulo(5,10,150,80,0,0,0);
		this->pantalla->dibujarRectangulo(5,120,150,80,0,0,0);
		this->pantalla->dibujarRectangulo(8,55,144,20,255,255,255);
		this->pantalla->dibujarRectangulo(8,165,144,20,255,255,255);
		this->pantalla->escribirTextoDesdePos("Usuario",10,10,40,blanco);
		this->pantalla->escribirTextoDesdePos("Contrasena",10,120,40,blanco);
		string usuarioTexto("sdfsdasdfsd");
		string contrasenaTexto("***");
		CasilleroTexto casillero=NINGUNO;
		this->pantalla->escribirTextoDesdePos(usuarioTexto.c_str(),13,50,25,negro);
		this->pantalla->escribirTextoDesdePos(contrasenaTexto.c_str(),13,160,25,negro);
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
						if(evento.button.x>=8 and evento.button.x<=152){
							if(evento.button.y>=55 and evento.button.y<=75){
								this->pantalla->dibujarRectangulo(8,55,144,20,200,200,200);
								this->pantalla->dibujarRectangulo(8,165,144,20,255,255,255);
								this->pantalla->escribirTextoDesdePos(usuarioTexto.c_str(),13,50,25,negro);
								this->pantalla->escribirTextoDesdePos(contrasenaTexto.c_str(),13,160,25,negro);
								casillero=USUARIO;
								this->actualizarPantalla();
							}
							else if(evento.button.y>=165 and evento.button.y<=185){
								this->pantalla->dibujarRectangulo(8,55,144,20,255,255,255);
								this->pantalla->dibujarRectangulo(8,165,144,20,200,200,200);
								this->pantalla->escribirTextoDesdePos(usuarioTexto.c_str(),13,50,25,negro);
								this->pantalla->escribirTextoDesdePos(contrasenaTexto.c_str(),13,160,25,negro);
								casillero=CONTRASENA;
								this->actualizarPantalla();
							}
						}
					}
				}
			}
		}
}
