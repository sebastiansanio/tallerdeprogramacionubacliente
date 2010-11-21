#include "Juego.h"

void * manejoEventos(void * juego_aux) {
	Juego * juego = (Juego *) juego_aux;
//	int x_nombre_jugador = 5, y_nombre_jugador = 220;
	SDL_Color rojo;
	rojo.r = 255;
	rojo.g = 0;
	rojo.b = 0;
	SDL_Color blanco;
	blanco.r = 255;
	blanco.g = 255;
	blanco.b = 255;
	int inicio = juego->infoconfig->ancho / 3.3;
	int distancia = juego->infoconfig->ancho / 6;
	int factor = juego->infoconfig->ancho / 42.5;
	int apuestaMax;
	string resultado;
	string idOperacion;
	int contadorOportunidades = 0;
	list<string>* operandos = new list<string> ();
	while (true) {
		if(!juego->enElTurno){
			SDL_Event evento;
			if (SDL_PollEvent(&evento)) {
				if (evento.type == SDL_QUIT) {
					juego->pantalla->escribirTextoDesdePos("CERRANDO",82,18,25,blanco);
					juego->pantalla->actualizarPantalla(82,18,100,40);
					break;
				} else if (evento.type == SDL_MOUSEBUTTONDOWN) {
					if (evento.button.button == 1) {
						if(evento.button.x < 80 and evento.button.y < 45){
							juego->pantalla->escribirTextoDesdePos("CERRANDO",82,18,25,blanco);
							juego->pantalla->actualizarPantalla(82,18,100,40);
							break;
						} else if (!juego->tipoJugador.jugadorVirtual) {
							juego->pantalla->escribirTextoDesdePos("No es tu turno", 5,juego->infoconfig->alto * (0.95), 24, rojo);
						}
					}

				}
			}
		}
	}
	juego->cerrar=true;
	delete operandos;
}

bool Juego::hayGanador(){
	return this->ganador!=" ";
}

void Juego::pedirGanador(){
	string idOperacion = "H";
	list<string>* operandos = new list<string> ();
	char* xml = parser->getXmlDeOperacion(idOperacion, operandos);
	cliente->enviar(xml);
	char * respuesta = cliente->recibirRespuesta();
	this->ganador = parserResultado->getPoso(respuesta);
	delete operandos;
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
	this->escenarioPedido=false;
	this->cerrar=false;
	this->pedirEscenario();
	this->cargarEscenario(this->escenario,false);
}

bool Juego::verificarResolucion(unsigned int alto,unsigned  int ancho){
	return((alto==480&&ancho==640) || (alto==600 && ancho==800) || (alto==768 && ancho==1024) || (alto==768 && ancho==1280));
}

void Juego::informarError(string idOperacion, string tipoError, string mensaje){
	list<string>* mensajesError = new list<string> ();
	mensajesError->push_back(tipoError);
	mensajesError->push_back(mensaje);
	this->parser->registrarError(idOperacion, mensajesError);
	delete mensajesError;
	cout << mensaje << endl;
}

Juego::~Juego() {
	delete pantalla;
	delete cliente;
	delete parser;
	delete parserResultado;
	delete infoconfig;
	delete imagenEscenario;
}

void Juego::pedirEscenario(){
	string ruta=PATHESCENARIO;
	//Pido el escenario
	string idOperacion="E";
	list<string>* operandos=new list<string>();
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	delete operandos;
	cliente->enviar(xml);
	cliente->recibirArchivo(ruta);
	this->escenario = ruta;
	this->escenarioPedido = true;
}

string Juego::pedirImagenJugador(Jugador * jugador){
	string ruta = jugador->getNombre() + ".bmp";
	jugador->setPath(ruta);
	//jugador->imagenEstablecida();
	//Pido la imagen del jugador
	string idOperacion = "I";
	list<string>* operandos = new list<string> ();
	list<string>::iterator it = operandos->begin();
	it = operandos->insert(it, "jugador");
	it++;
	it = operandos->insert(it, jugador->getNombre());
	char* xml = parser->getXmlDeOperacion(idOperacion, operandos);
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

void Juego::cargarEscenario(string path,bool cargado){
	if(cargado){
		delete this->imagenEscenario;
	}
	this->imagenEscenario = new BitMap(path);
	if((this->imagenEscenario->esUnaImagenCorrecta())and(this->imagenEscenario->getAlto()>1)and(this->imagenEscenario->getAncho()>1)){
		this->imagenEscenario->resizeTo(this->infoconfig->alto, this->infoconfig->ancho);
//		pantalla->dibujarBitMapDesdePos(escenario),0,0);
	}else{
		this->informarError("B","E","El escenario no es una imagen BMP o esta corrupta");
	}
}

void Juego::dibujarPantalla(string path){
	BitMap* pantallaADibujar = new BitMap(path);
	if((pantallaADibujar->esUnaImagenCorrecta())and(pantallaADibujar->getAlto()>1)and(pantallaADibujar->getAncho()>1)){
		pantallaADibujar->resizeTo(this->infoconfig->alto, this->infoconfig->ancho);
		pantalla->dibujarBitMapDesdePos(pantallaADibujar,0,0);
	}else{
		this->informarError("B","E","El escenario no es una imagen BMP o esta corrupta");
	}
	delete pantallaADibujar;
}

void Juego::dibujarEscenario(){
	pantalla->dibujarBitMapDesdePos(this->imagenEscenario,0,0);
}


void Juego::dibujarJugador(Jugador jugadorADibujar,string jugadorTurno){
	BitMap* jugador = new BitMap(jugadorADibujar.getPath());
	SDL_Color color;
	color.r=240;
	color.b=55;
	color.g=200;
	bool esTurnoJugador=(jugadorADibujar.getNombre()==jugadorTurno);
	const char* flecha="<";
	//dibujaria con el sdl_ttf el nombre y la plata al lado de la imagen
	if ((jugador->esUnaImagenCorrecta())and(jugador->getAlto()>1)and(jugador->getAncho()>1)) {
		int tamImagen = this->infoconfig->ancho / 10;
		jugador->resizeTo(tamImagen, tamImagen);
		const char* nombre = jugadorADibujar.getNombre().c_str();
		string platasignada = "$"+jugadorADibujar.getPlata();
		const char* plata = platasignada.c_str();
		int tamfuente = this->infoconfig->ancho / 32.5;
		int factor = this->infoconfig->alto / 110;
		if (jugadorADibujar.getId() == 1) {
			pantalla->dibujarBitMapDesdePosCircular(jugador,
					this->infoconfig->ancho / 8, this->infoconfig->alto / 2);
			pantalla->escribirTextoDesdePos(nombre, (this->infoconfig->ancho / 8)+(tamImagen/4), (this->infoconfig->alto / 2)+tamImagen+factor, tamfuente, color );
			pantalla->escribirTextoDesdePos(plata, (this->infoconfig->ancho / 8)+(tamImagen/4), (this->infoconfig->alto / 2)+tamImagen+factor+tamfuente, tamfuente,color );
			if (esTurnoJugador){
				pantalla->escribirTextoDesdePos(flecha, (this->infoconfig->ancho / 8)+tamImagen,(this->infoconfig->alto / 2)+(tamImagen/8), tamfuente*2.5, color );
			}
		} else if (jugadorADibujar.getId() == 2) {
			pantalla->dibujarBitMapDesdePosCircular(jugador,
					this->infoconfig->ancho / 5.45, this->infoconfig->alto / 3.2);
			pantalla->escribirTextoDesdePos(nombre, (this->infoconfig->ancho / 5.45)+(tamImagen/4), (this->infoconfig->alto / 3.2)-(2*tamfuente)-factor, tamfuente ,color);
			pantalla->escribirTextoDesdePos(plata, (this->infoconfig->ancho / 5.45)+(tamImagen/4), (this->infoconfig->alto / 3.2)-(tamfuente)-factor, tamfuente,color );
			if (esTurnoJugador){
				pantalla->escribirTextoDesdePos(flecha, (this->infoconfig->ancho / 5.45)+tamImagen,(this->infoconfig->alto / 3.2)+(tamImagen/8), tamfuente*2, color );
			}
		} else if (jugadorADibujar.getId() == 3) {
			pantalla->dibujarBitMapDesdePosCircular(jugador,
					this->infoconfig->ancho / 2.65, this->infoconfig->alto / 3.9);
			pantalla->escribirTextoDesdePos(nombre, (this->infoconfig->ancho / 2.65)+(tamImagen/4), (this->infoconfig->alto / 3.9)-(2*tamfuente)-factor, tamfuente ,color);
			pantalla->escribirTextoDesdePos(plata, (this->infoconfig->ancho / 2.65)+(tamImagen/4), (this->infoconfig->alto / 3.9)-(tamfuente)-factor, tamfuente ,color);
			if (esTurnoJugador){
				pantalla->escribirTextoDesdePos(flecha, (this->infoconfig->ancho / 2.65)+tamImagen,(this->infoconfig->alto / 3.9)+(tamImagen/8), tamfuente*2, color );
			}
		} else if (jugadorADibujar.getId() == 4) {
			pantalla->dibujarBitMapDesdePosCircular(jugador,
					this->infoconfig->ancho / 1.7, this->infoconfig->alto / 3.9);
			pantalla->escribirTextoDesdePos(nombre, (this->infoconfig->ancho / 1.7)+(tamImagen/4), (this->infoconfig->alto / 3.9)-(2*tamfuente)-factor, tamfuente,color );
			pantalla->escribirTextoDesdePos(plata, (this->infoconfig->ancho / 1.7)+(tamImagen/4), (this->infoconfig->alto / 3.9)-(tamfuente)-factor, tamfuente,color );
			if (esTurnoJugador){
				pantalla->escribirTextoDesdePos(flecha, (this->infoconfig->ancho / 1.7)+tamImagen,(this->infoconfig->alto / 3.9)+(tamImagen/8), tamfuente*2, color );
			}
		} else if (jugadorADibujar.getId() == 5) {
			pantalla->dibujarBitMapDesdePosCircular(jugador,
					this->infoconfig->ancho / 1.4, this->infoconfig->alto / 3.2);
			pantalla->escribirTextoDesdePos(nombre, (this->infoconfig->ancho / 1.4)+(tamImagen/4), (this->infoconfig->alto / 3.2)-(2*tamfuente)-factor, tamfuente,color );
			pantalla->escribirTextoDesdePos(plata, (this->infoconfig->ancho / 1.4)+(tamImagen/4), (this->infoconfig->alto / 3.2)-(tamfuente)-factor, tamfuente,color );
			if (esTurnoJugador){
				pantalla->escribirTextoDesdePos(flecha, (this->infoconfig->ancho / 1.4)+tamImagen,(this->infoconfig->alto / 3.2)+(tamImagen/8), tamfuente*2, color );
			}
		} else if (jugadorADibujar.getId() == 6) {
			pantalla->dibujarBitMapDesdePosCircular(jugador,
					this->infoconfig->ancho / 1.3, this->infoconfig->alto / 2);
			pantalla->escribirTextoDesdePos(nombre, (this->infoconfig->ancho / 1.3)+(tamImagen/4), (this->infoconfig->alto / 2)+tamImagen+factor, tamfuente,color );
			pantalla->escribirTextoDesdePos(plata, (this->infoconfig->ancho / 1.3)+(tamImagen/4), (this->infoconfig->alto / 2)+tamImagen+factor+tamfuente, tamfuente,color );
			if (esTurnoJugador){
				pantalla->escribirTextoDesdePos(flecha, (this->infoconfig->ancho / 1.3)+tamImagen,(this->infoconfig->alto / 2)+(tamImagen/8), tamfuente*2, color );
			}
		}
	} else {
		this->informarError("B","E","El jugador " + jugadorADibujar.getNombre()+ " no es una imagen BMP o esta corrupta");
			}
	delete jugador;
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
		if(nombre.compare(this->nombreJugador)==0){
			this->idJugador=i+1;
			this->plataJugador = atoi(plata.c_str());
		}
		iterador=jugadores->insert(iterador,Jugador("",nombre,plata,i+1));
		iterador++;
	}
	this->jugadores = jugadores;
	return jugadores;
}

void Juego::dibujarCarta(Carta cartaADibujar){
	BitMap* carta = new BitMap("Cartas/" + cartaADibujar.getPalo()+"-"+cartaADibujar.getNumero()+".bmp");
	if ((carta->esUnaImagenCorrecta())and(carta->getAlto()>1)and(carta->getAncho()>1)) {
		int tamImagen = this->infoconfig->ancho / 22;
		carta->resizeTo(2.5 * tamImagen,2*tamImagen);
		int id = cartaADibujar.getId();
		if (id == 1) {
			pantalla->dibujarBitMapDesdePos(carta,
					this->infoconfig->ancho / 3.15, (this->infoconfig->alto / 2.3));
		} else if (id == 2) {
			pantalla->dibujarBitMapDesdePos(carta,
					this->infoconfig->ancho / 2.58, (this->infoconfig->alto / 2.3));
		} else if (id == 3) {
			pantalla->dibujarBitMapDesdePos(carta,
					this->infoconfig->ancho / 2.2, (this->infoconfig->alto / 2.3));
		} else if (id == 4) {
			pantalla->dibujarBitMapDesdePos(carta,
					this->infoconfig->ancho / 1.88, (this->infoconfig->alto / 2.3));
		} else if (id == 5) {
			pantalla->dibujarBitMapDesdePos(carta,
					this->infoconfig->ancho / 1.66, (this->infoconfig->alto / 2.3));
		}
	}else {
		this->informarError("B","E","La carta " + cartaADibujar.getNumero() + " de " + cartaADibujar.getPalo() +	" no tiene un imagen BMP o esta corrupta");
		}
	delete carta;
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
	delete operandos;
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
		if(jugador->getNombre() == this->nombreJugador)
			this->cartasJugador = cartas;
		jugador->setCartas(cartas);
	}
	delete operandos;
	return cartas;
}
list<Carta>* Juego::getCartasJugador(){
	return this->cartasJugador;
}
void Juego::dibujarCartasJugadores(){
	SDL_Color rojo;
	rojo.r = 255;
	rojo.g = 0;
	rojo.b = 0;

	if (!jugadores->empty()) {
		list<Jugador>::iterator it = jugadores->begin();
		for (int i=0;i<jugadores->size();i++){
			this->dibujarCartaJugador(&(*it));
			it++;
		}
	} else {
		this->pantalla->escribirTextoDesdePos("No hay jugadores", 5, this->infoconfig->alto * (0.95), 24, rojo);
	}

}

void Juego::dibujarCartaJugador(Jugador * jugador){
//	SDL_Color blanco;
//	blanco.r = 255;
//	blanco.g = 255;
//	blanco.b = 255;
//	this->pantalla->escribirTextoDesdePos("Cartas de:",this->infoconfig->ancho/2 - 90,2,30,blanco);
//	this->pantalla->escribirTextoDesdePos(jugador->getNombre().c_str(),this->infoconfig->alto/2 + 110,2,30,blanco);
	Carta *carta1, *carta2;
	int id = jugador->getId();
	if((this->tipoJugador.jugadorObservador) or (id==this->idJugador)){
        if(jugador->getCartas()!=NULL){
            carta1 = &jugador->getCartas()->front();
            carta2 = &jugador->getCartas()->back();
        }else{
            carta1= new Carta("","Imagen","Carta",1);
            carta2= new Carta("","Imagen","Carta",1);
        }
	} else {
		carta1= new Carta("","Imagen","Carta",1);
		carta2= new Carta("","Imagen","Carta",1);
	}
	BitMap* imagen_carta1 = new BitMap("Cartas/" + carta1->getPalo() + "-"
			+ carta1->getNumero() + ".bmp");
	BitMap* imagen_carta2 = new BitMap("Cartas/" + carta2->getPalo() + "-"
			+ carta2->getNumero() + ".bmp");
	unsigned int ancho = this->infoconfig->ancho;
	if ((imagen_carta1->esUnaImagenCorrecta()) and (imagen_carta1->getAlto()
			> 1) and (imagen_carta1->getAncho() > 1)) {
		//imagen_carta1->resizeTo(this->infoconfig->alto/5,this->infoconfig->ancho/5);
		imagen_carta1->recortarImagen(this->infoconfig->alto / 46,
				this->infoconfig->alto / 20, this->infoconfig->alto / 15.8,
				this->infoconfig->alto / 6.5);
		if (id == 1) {
			this->pantalla->dibujarBitMapDesdePos(imagen_carta1, ancho / 20.2, ancho / 2.65);
		} else if (id == 2) {
			this->pantalla->dibujarBitMapDesdePos(imagen_carta1, ancho / 9.88, ancho / 4);
		} else if (id == 3) {
			this->pantalla->dibujarBitMapDesdePos(imagen_carta1, ancho / 3.45, ancho / 4.62);
		} else if (id == 4) {
			this->pantalla->dibujarBitMapDesdePos(imagen_carta1, ancho / 2, ancho / 4.62);
		} else if (id == 5) {
			this->pantalla->dibujarBitMapDesdePos(imagen_carta1, ancho / 1.21, ancho / 4);
		} else if (id == 6) {
			this->pantalla->dibujarBitMapDesdePos(imagen_carta1, ancho / 1.14, ancho / 2.65);
		}
	} else {
		this->informarError("B", "E", "La carta " + carta1->getNumero() + " de "
				+ carta1->getPalo() + " no tiene un imagen BMP o esta corrupta");
	}
	if ((imagen_carta2->esUnaImagenCorrecta()) and (imagen_carta2->getAlto()
			> 1) and (imagen_carta2->getAncho() > 1)) {
		//imagen_carta2->resizeTo(this->infoconfig->alto/5,this->infoconfig->ancho/5);
		imagen_carta2->recortarImagen(this->infoconfig->alto / 46,
				this->infoconfig->alto / 20, this->infoconfig->alto / 15.8,
				this->infoconfig->alto / 6.5);
		int dist = ancho / 28;
		if (id == 1) {
			this->pantalla->dibujarBitMapDesdePos(imagen_carta2,
					(ancho / 20.2) + dist, ancho / 2.65);
		} else if (id == 2) {
			this->pantalla->dibujarBitMapDesdePos(imagen_carta2,
					(ancho / 9.88) + dist, ancho / 4);
		} else if (id == 3) {
			this->pantalla->dibujarBitMapDesdePos(imagen_carta2,
					(ancho / 3.45) + dist, ancho / 4.62);
		} else if (id == 4) {
			this->pantalla->dibujarBitMapDesdePos(imagen_carta2, (ancho / 2)
					+ dist, ancho / 4.62);
		} else if (id == 5) {
			this->pantalla->dibujarBitMapDesdePos(imagen_carta2,
					(ancho / 1.21) + dist, ancho / 4);
		} else if (id == 6) {
			this->pantalla->dibujarBitMapDesdePos(imagen_carta2,
					(ancho / 1.14) + dist, ancho / 2.65);
		}
	} else {
		this->informarError("B", "E", "La carta " + carta2->getNumero()
				+ " de " + carta2->getPalo()
				+ " no tiene un imagen BMP o esta corrupta");
	}
	delete imagen_carta1;
	delete imagen_carta2;
}

bool Juego::validarJugador(string usuario, string pass, int *plata, int *cargado){
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
	bool valido=this->parserResultado->getPlataYCargado(respuesta,plata,cargado);
	return valido;
}

bool Juego::comenzarPartida(){
	string idOperacion="M";
	list<string>* operandos=new list<string>();
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	delete operandos;
	cliente->enviar(xml);
	char * respuesta = cliente->recibirRespuesta();
	return (this->parserResultado->DecodificaResultado(respuesta));
}

void Juego::cargarFichas(string monto,string usuario){
	string idOperacion="N";
	list<string>* operandos=new list<string>();
	list<string>::iterator it=operandos->begin();
	it=operandos->insert(it,"usuario");
	it++;
	it=operandos->insert(it,usuario);
	it++;
	it=operandos->insert(it,"monto");
	it++;
	it=operandos->insert(it,monto);
	char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
	delete operandos;
	cliente->enviar(xml);
	char * respuesta = cliente->recibirRespuesta();
	this->parserResultado->DecodificaResultado(respuesta);
}

bool Juego::registrarJugador(string usuario, string pass, string ruta){
	//Antes de registrarlo se fija si el archivo existe
	fstream  archivo;
	string path(ruta);
	path+= ".bmp";
	archivo.open(path.c_str(), fstream::in | fstream::binary );
	if(!archivo.good()){
		cout<<"Ingreso mal el path: " + path<<endl;
		archivo.close();
		return false;
	}
	archivo.close();
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
		string jugador(usuario);
		if(enviarImagenJugador(ruta,jugador)){
			cout<<"Archivo enviado"<<endl;
		}
		return true;
	}
	delete operandos;
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
		int tamImagenalto = this->infoconfig->alto / 5.5;
		int tamfuente = this->infoconfig->ancho / 42;
		int factor = this->infoconfig->ancho / 100;
		int distancia = this->infoconfig->ancho / 6;
		int inicio = this->infoconfig->ancho / 3.3;
		const char* texto = textoBoton.c_str();
		boton->resizeTo(tamImagenalto, tamImagenancho);
		if (pos == 0) {
			pantalla->dibujarBitMapDesdePosCircular(boton, inicio,
					(this->infoconfig->alto / 1.3));
			pantalla->escribirTextoDesdePos(texto, inicio + factor,
					(this->infoconfig->alto / 1.3) + (tamImagenalto / 2)
							- (tamfuente / 2), tamfuente, color);
		} else if (pos == 1) {
			pantalla->dibujarBitMapDesdePosCircular(boton, inicio + pos * distancia,
					(this->infoconfig->alto / 1.3));
			pantalla->escribirTextoDesdePos(texto, inicio + pos * distancia
					+ factor, (this->infoconfig->alto / 1.3) + (tamImagenalto
					/ 2) - (tamfuente / 2), tamfuente, color);
		} else if (pos == 2) {
			pantalla->dibujarBitMapDesdePosCircular(boton, inicio + pos * distancia,
					(this->infoconfig->alto / 1.3));
			pantalla->escribirTextoDesdePos(texto, inicio + pos * distancia
					+ factor, (this->infoconfig->alto / 1.3) + (tamImagenalto
					/ 2) - (tamfuente / 2), tamfuente, color);
		} else if (pos == 3) {
			pantalla->dibujarBitMapDesdePosCircular(boton, inicio + pos * distancia,
					(this->infoconfig->alto / 1.3));
			pantalla->escribirTextoDesdePos(texto, inicio + pos * distancia
					+ factor, (this->infoconfig->alto / 1.3) + (tamImagenalto
					/ 2) - (tamfuente / 2), tamfuente, color);
		}
	} else {
		this->informarError("B", "E",
				"Uno de los botones no es una imagen BMP o esta corrupta");
	}
	delete boton;
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
	this->dibujarPantalla("boton.bmp");
	this->pantalla->escribirTextoDesdePos("Loguearse",10,10,40,blanco);
	this->pantalla->escribirTextoDesdePos("Registrarse",10,100,40,blanco);
	this->pantalla->escribirTextoDesdePos("Observar",10,190,40,blanco);
	this->pantalla->escribirTextoDesdePos("Estadisticas",10,280,40,blanco);
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
							this->dibujarPantallaLogin(false,0,true);
							terminar=true;
						}
						else if(evento.button.y>=280 and evento.button.y<=335){
						//							this->dibujarPantallaLogin(false,0,true);
													this->dibujarPantallaEstadistica();
													terminar=true;
						}
					}
				}
			}
		}
	}
}


void Juego::dibujarPantallaEstadistica(){
	this->pantalla->dibujarRectangulo(0,0,0,0,255,255,255);
	SDL_Color blanco;
	blanco.r=255;
	blanco.g=255;
	blanco.b=255;
	this->dibujarPantalla("boton.bmp");
	this->pantalla->escribirTextoDesdePos("Evolucion por usuarios ",10,10,40,blanco);
	this->pantalla->escribirTextoDesdePos("Evolucion por usuarios conectados",10,100,40,blanco);
	this->pantalla->escribirTextoDesdePos("Listado de los usuarios registrados",10,190,40,blanco);
	this->pantalla->escribirTextoDesdePos("Listado de los usuarios conectados",10,280,40,blanco);
	this->pantalla->escribirTextoDesdePos("Ranking de usuarios",10,370,40,blanco);
	this->pantalla->escribirTextoDesdePos("Volver",10,460,40,blanco);
	//this->pantalla->escribirTextoDesdePos("Observar",10,190,40,blanco);
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
					if(evento.button.x>=5 and evento.button.x<=255){
						if(evento.button.y>=10 and evento.button.y<=65){
							this->dibujarPantallaEvolucion("evolucionUsuario", "registrados");
							terminar=true;
						}
						else if(evento.button.y>=100 and evento.button.y<=155){
							this->dibujarPantallaEvolucion("evolucionUsuarioConectado", "conectados");
							terminar=true;
						}
						else if(evento.button.y>=190 and evento.button.y<=245){
							this->dibujarPantallaListadoRegistrados();
							terminar=true;
						}
						else if(evento.button.y>=280 and evento.button.y<=335){
							this->dibujarPantallaListadoConectados();
							terminar=true;
						}
						else if(evento.button.y>=370 and evento.button.y<=425){
							this->dibujarPantallaRanking();
													terminar=true;
						}
						else if(evento.button.y>=460 and evento.button.y<=515){
							this->dibujarPantallaPrincipal();
							terminar=true;
						}
					}
				}
			}
		}
	}
}


list<string>* Juego::getEstadisticas(string tipoEstadistica){
	string idOperacion="K";
		list<string>* operandos=new list<string>();
		list<string>::iterator it = operandos->begin();
		it = operandos->insert(it, "tipoEstadistica");
		it++;
		it = operandos->insert(it, tipoEstadistica);

		char* xml=parser->getXmlDeOperacion(idOperacion,operandos);
		delete operandos;
		cliente->enviar(xml);
		char * respuesta=cliente->recibirRespuesta();

		list<string>* usuarios=parserResultado->getJugadores(respuesta);
		return usuarios;
}

void Juego::dibujarPantallaRanking(){
	this->pantalla->dibujarRectangulo(0,0,0,0,255,255,255);
	SDL_Color blanco;
	blanco.r=255;
	blanco.g=255;
	blanco.b=255;
	list <string> * jugadores=this->getEstadisticas("ranking");
	int j=100;
	this->dibujarPantalla("boton.bmp");
	this->pantalla->escribirTextoDesdePos("Ranking de usuarios ",30,10,40,blanco);
	this->pantalla->escribirTextoDesdePos("Usuario",30,100,40,blanco);
	this->pantalla->escribirTextoDesdePos("Fichas",240,100,40,blanco);
	this->dibujarListaJugadores(jugadores,j);
	this->pantalla->escribirTextoDesdePos("Volver ",550,10,40,blanco);
	this->actualizarPantalla();
	bool terminar=false;
	SDL_Event evento;
	while(!terminar){
		if(SDL_PollEvent(&evento)) {
			if(evento.type == SDL_QUIT){
				exit(0);
			} else if(evento.type == SDL_MOUSEBUTTONDOWN){
				if(evento.button.button==1){
					if(evento.button.x>=545 and evento.button.x<=590){
						if(evento.button.y>=10 and evento.button.y<=65){
							this->dibujarPantallaEstadistica();
							//this->jugar(jugador_observador,false);
							terminar=true;
						}

					}
				}
			} else if(evento.type == SDL_KEYDOWN){
				if(evento.key.keysym.sym == SDLK_UP){
					j+=70;
					this->dibujarPantalla("boton.bmp");
					this->pantalla->escribirTextoDesdePos("Ranking de usuarios ",30,10,40,blanco);
					this->pantalla->escribirTextoDesdePos("Usuario",30,100,40,blanco);
					this->pantalla->escribirTextoDesdePos("Fichas",240,100,40,blanco);
					this->dibujarListaJugadores(jugadores,j);
					this->pantalla->escribirTextoDesdePos("Volver ",550,10,40,blanco);
					this->actualizarPantalla();
				} else if(evento.key.keysym.sym == SDLK_DOWN){
					j-=70;
					this->dibujarPantalla("boton.bmp");
					this->pantalla->escribirTextoDesdePos("Ranking de usuarios ",30,10,40,blanco);
					this->pantalla->escribirTextoDesdePos("Usuario",30,100,40,blanco);
					this->pantalla->escribirTextoDesdePos("Fichas",240,100,40,blanco);
					this->dibujarListaJugadores(jugadores,j);
					this->pantalla->escribirTextoDesdePos("Volver ",550,10,40,blanco);
					this->actualizarPantalla();
				}
			}
		}
	}
}

void Juego::dibujarListaJugadores(list <string> * jugadores, int j){
	SDL_Color blanco;
	blanco.r=255;
	blanco.g=255;
	blanco.b=255;
	list<string>::iterator iterador=jugadores->begin();
	while(iterador != jugadores->end()){
		j=j+70;
		if(j>100){
			string nom=(*iterador);
			this->pantalla->escribirTextoDesdePos(nom.c_str(),30,j,40,blanco);
		}
		iterador++;
		if(j>100){
			string ficha=(*iterador);
			this->pantalla->escribirTextoDesdePos(ficha.c_str(),240,j,40,blanco);
		}
		iterador++;
	}
}

void Juego::dibujarPantallaEvolucion(string listado, string tipo){
	SDL_Color blanco;
	blanco.r=255;
	blanco.g=255;
	blanco.b=255;
	this->pantalla->dibujarRectangulo(0,0,0,0,255,255,255);
	list <string> * jugadores=this->getEstadisticas(listado);
	int j=100;
	this->dibujarPantalla("boton.bmp");
	string titulo = "Evolucion de los usuarios "+ tipo;
	this->pantalla->escribirTextoDesdePos(titulo.c_str(),30,10,40,blanco);
	this->pantalla->escribirTextoDesdePos("Fecha",30,100,40,blanco);
	this->pantalla->escribirTextoDesdePos(tipo.c_str(),240,100,40,blanco);
	this->dibujarListaJugadores(jugadores, j);
	this->pantalla->escribirTextoDesdePos("Volver ",550,10,40,blanco);
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
					if(evento.button.x>=545 and evento.button.x<=590){
						if(evento.button.y>=10 and evento.button.y<=65){
							this->dibujarPantallaEstadistica();
							terminar=true;
						}

					}
				}
			} else if(evento.type == SDL_KEYDOWN){
				if(evento.key.keysym.sym==SDLK_UP){
					j+=70;
					this->dibujarPantalla("boton.bmp");
					string titulo = "Evolucion de los usuarios "+ tipo;
					this->pantalla->escribirTextoDesdePos(titulo.c_str(),30,10,40,blanco);
					this->pantalla->escribirTextoDesdePos("Fecha",30,100,40,blanco);
					this->pantalla->escribirTextoDesdePos(tipo.c_str(),240,100,40,blanco);
					this->dibujarListaJugadores(jugadores, j);
					this->pantalla->escribirTextoDesdePos("Volver ",550,10,40,blanco);
					this->actualizarPantalla();

				} else if(evento.key.keysym.sym==SDLK_DOWN){
					j-=70;
					this->dibujarPantalla("boton.bmp");
					string titulo = "Evolucion de los usuarios "+ tipo;
					this->pantalla->escribirTextoDesdePos(titulo.c_str(),30,10,40,blanco);
					this->pantalla->escribirTextoDesdePos("Fecha",30,100,40,blanco);
					this->pantalla->escribirTextoDesdePos(tipo.c_str(),240,100,40,blanco);
					this->dibujarListaJugadores(jugadores, j);
					this->pantalla->escribirTextoDesdePos("Volver ",550,10,40,blanco);
					this->actualizarPantalla();
				}
			}
		}
	}
}

void Juego::dibujarPantallaListado(string listado){
	this->pantalla->dibujarRectangulo(0,0,0,0,255,255,255);
	SDL_Color blanco;
	blanco.r=255;
	blanco.g=255;
	blanco.b=255;
	list <string> * jugadores=this->getEstadisticas(listado);
	int j=100;
	this->dibujarPantalla("boton.bmp");
	this->pantalla->escribirTextoDesdePos("Listado de usuarios ",30,10,40,blanco);
	this->pantalla->escribirTextoDesdePos("Usuario",30,100,40,blanco);
	this->pantalla->escribirTextoDesdePos("Fecha",240,100,40,blanco);
	this->dibujarListaJugadores(jugadores,j);
	this->pantalla->escribirTextoDesdePos("Volver ",550,10,40,blanco);
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
					if(evento.button.x>=545 and evento.button.x<=590){
						if(evento.button.y>=10 and evento.button.y<=65){
							this->dibujarPantallaEstadistica();
							//this->jugar(jugador_observador,false);
							terminar=true;
						}
					}
				}
			} else if(evento.type == SDL_KEYDOWN){
				if(evento.key.keysym.sym == SDLK_UP){
					j+=70;
					this->dibujarPantalla("boton.bmp");
					this->pantalla->escribirTextoDesdePos("Listado de usuarios ",30,10,40,blanco);
					this->pantalla->escribirTextoDesdePos("Usuario",30,100,40,blanco);
					this->pantalla->escribirTextoDesdePos("Fecha",240,100,40,blanco);
					this->dibujarListaJugadores(jugadores,j);
					this->pantalla->escribirTextoDesdePos("Volver ",550,10,40,blanco);
					this->actualizarPantalla();
				} else if(evento.key.keysym.sym == SDLK_DOWN){
					j-=70;
					this->dibujarPantalla("boton.bmp");
					this->pantalla->escribirTextoDesdePos("Listado de usuarios ",30,10,40,blanco);
					this->pantalla->escribirTextoDesdePos("Usuario",30,100,40,blanco);
					this->pantalla->escribirTextoDesdePos("Fecha",240,100,40,blanco);
					this->dibujarListaJugadores(jugadores,j);
					this->pantalla->escribirTextoDesdePos("Volver ",550,10,40,blanco);
					this->actualizarPantalla();
				}
			}
		}
	}
}

void Juego::dibujarPantallaListadoRegistrados(){
	this->pantalla->dibujarRectangulo(0,0,0,0,255,255,255);
	SDL_Color blanco;
	blanco.r=255;
	blanco.g=255;
	blanco.b=255;
	this->dibujarPantalla("boton.bmp");
	this->pantalla->escribirTextoDesdePos("Listado de los usuarios registrados",10,10,40,blanco);
	this->pantalla->escribirTextoDesdePos("Ultimo dia",10,100,40,blanco);
	this->pantalla->escribirTextoDesdePos("Ultimo mes",10,190,40,blanco);
	this->pantalla->escribirTextoDesdePos("Ultimo año",10,280,40,blanco);
	this->pantalla->escribirTextoDesdePos("Volver",10,460,40,blanco);
	//this->pantalla->escribirTextoDesdePos("Observar",10,190,40,blanco);
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
						 if(evento.button.y>=100 and evento.button.y<=155){
							this->dibujarPantallaListado("listadoRegistradoDia");
							terminar=true;
						}
						else if(evento.button.y>=190 and evento.button.y<=245){
							this->dibujarPantallaListado("listadoRegistradoMes");
							//this->jugar(true,false);
							terminar=true;
						}
						else if(evento.button.y>=280 and evento.button.y<=335){
							this->dibujarPantallaListado("listadoRegistradoAnio");
							terminar=true;
						}
						else if(evento.button.y>=460 and evento.button.y<=515){
							this->dibujarPantallaEstadistica();
							terminar=true;
						}
					}
				}
			}
		}
	}
}


void Juego::dibujarPantallaListadoConectados(){
	this->pantalla->dibujarRectangulo(0,0,0,0,255,255,255);
	SDL_Color blanco;
	blanco.r=255;
	blanco.g=255;
	blanco.b=255;
	this->dibujarPantalla("boton.bmp");
	this->pantalla->escribirTextoDesdePos("Listado de los usuarios conectados",10,10,40,blanco);
	this->pantalla->escribirTextoDesdePos("Ultimo dia",10,100,40,blanco);
	this->pantalla->escribirTextoDesdePos("Ultimo mes",10,190,40,blanco);
	this->pantalla->escribirTextoDesdePos("Ultimo año",10,280,40,blanco);
	this->pantalla->escribirTextoDesdePos("Volver",10,460,40,blanco);
	//this->pantalla->escribirTextoDesdePos("Observar",10,190,40,blanco);
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
						 if(evento.button.y>=100 and evento.button.y<=155){
							 this->dibujarPantallaListado("listadoConectadoDia");
							terminar=true;
						}
						else if(evento.button.y>=190 and evento.button.y<=245){
							this->dibujarPantallaListado("listadoConectadoMes");
							terminar=true;
						}
						else if(evento.button.y>=280 and evento.button.y<=335){
							this->dibujarPantallaListado("listadoConectadoAnio");
							terminar=true;
						}

						else if(evento.button.y>=460 and evento.button.y<=515){
							this->dibujarPantallaEstadistica();
							terminar=true;
						}
					}
				}
			}
		}
	}
}



void Juego::dibujarPantallaLogin(bool usuarioIncorrecto, int cantidadIntentos, bool jugador_observador){
	this->dibujarPantalla("boton.bmp");
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
	this->pantalla->dibujarRectangulo(100,245,20,20,255,255,255);
	this->pantalla->dibujarRectangulo(8,365,80,30,255,255,255);
	this->pantalla->escribirTextoDesdePos("Usuario",10,10,40,blanco);
	this->pantalla->escribirTextoDesdePos("Contrasena",10,120,40,blanco);
	this->pantalla->escribirTextoDesdePos("Virtual",10,230,40,blanco);
	this->pantalla->escribirTextoDesdePos("Volver",10,355,40,negro);
	string usuarioTexto("");
	string contrasenaTexto("");
	string contrasenaAsteriscos("");
	bool jugadorVirtual=false;
	CasilleroTexto casillero=NINGUNO;
	this->pantalla->escribirStringDesdePos(usuarioTexto,13,50,25,0,0,0);
	this->pantalla->escribirStringDesdePos(contrasenaAsteriscos,13,160,25,0,0,0);
	this->actualizarPantalla();
	char caracterLeido=(char)0;
	bool terminar=false;
	int plata=0;
	int cargado=0;
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
							this->pantalla->escribirStringDesdePos(usuarioTexto,13,50,25,0,0,0);
							this->pantalla->escribirStringDesdePos(contrasenaAsteriscos,13,160,25,0,0,0);
							casillero=USUARIO;
							this->actualizarPantalla();
						}
						else if(evento.button.y>=165 and evento.button.y<=185){
							this->pantalla->dibujarRectangulo(8,55,144,20,255,255,255);
							this->pantalla->dibujarRectangulo(8,165,144,20,200,200,200);
							this->pantalla->escribirStringDesdePos(usuarioTexto,13,50,25,0,0,0);
							this->pantalla->escribirStringDesdePos(contrasenaAsteriscos,13,160,25,0,0,0);
							casillero=CONTRASENA;
							this->actualizarPantalla();
						}
					}
					if (evento.button.x>=100 and evento.button.x<120 and evento.button.y>=245 and evento.button.y<265){
						if(jugadorVirtual){
							jugadorVirtual=false;
							this->pantalla->dibujarRectangulo(100,245,20,20,255,255,255);
							this->actualizarPantalla();
						} else {
							jugadorVirtual=true;
							this->pantalla->dibujarRectangulo(100,245,20,20,255,0,0);
							this->actualizarPantalla();
						}
					} if(evento.button.x>=8 and evento.button.x<=88){
						if(evento.button.y>=365 and evento.button.y<=395){
							this->dibujarPantallaPrincipal();
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
							this->pantalla->escribirStringDesdePos(usuarioTexto,13,50,25,0,0,0);
							this->actualizarPantalla();
						}
					} else if(casillero==CONTRASENA){
						int nuevoTamanio=contrasenaTexto.size()-1;
						if (nuevoTamanio>=0){
							contrasenaTexto.resize(contrasenaTexto.size()-1);
							contrasenaAsteriscos.resize(contrasenaAsteriscos.size()-1);
							this->pantalla->dibujarRectangulo(8,165,144,20,200,200,200);
							this->pantalla->escribirStringDesdePos(contrasenaAsteriscos,13,160,25,0,0,0);
							this->actualizarPantalla();
						}
					}
				} else if(evento.key.keysym.sym==SDLK_RETURN){
					if(!this->validarJugador(usuarioTexto,contrasenaTexto,&plata,&cargado)){
						this->dibujarPantallaLogin(true,cantidadIntentos + 1,jugador_observador);
					}else{
						this->nombreJugador=usuarioTexto;
						this->pantalla->dibujarRectangulo(0,this->infoconfig->alto*(0.9),this->infoconfig->ancho,24,255,255,255);
						this->pantalla->escribirTextoDesdePos("Se logueo con exito",5,this->infoconfig->alto*(0.9),24,rojo);
						this->actualizarPantalla();
						sleep(0.5);
						this->dibujarPantallaComienzo(plata<=100,2000-cargado,usuarioTexto);
						this->comenzarPartida();
						this->jugar(jugador_observador,jugadorVirtual);
					}
					//entre la a y la z o entre el 0 y el 9
				} else if(((evento.key.keysym.sym>=97) and (evento.key.keysym.sym<=122) )or((evento.key.keysym.sym>=48) and (evento.key.keysym.sym<=57) )){
					caracterLeido=(char)evento.key.keysym.unicode;
					if(casillero==USUARIO){
						usuarioTexto+=caracterLeido;
//						cout<<"caracter leido/"<<caracterLeido<<"/"<<endl;
//						cout<<usuarioTexto<<endl;
						this->pantalla->dibujarRectangulo(8,55,144,20,200,200,200);
						this->pantalla->escribirStringDesdePos(usuarioTexto,13,50,25,0,0,0);
						this->actualizarPantalla();
					}
					if(casillero==CONTRASENA){
						contrasenaTexto+=caracterLeido;
						contrasenaAsteriscos+="*";
						this->pantalla->dibujarRectangulo(8,165,144,20,200,200,200);
						this->pantalla->escribirStringDesdePos(contrasenaAsteriscos,13,160,25,0,0,0);
						this->actualizarPantalla();
					}
				}
			}
		}
	}
}

void Juego::dibujarPantallaComienzo(bool carga,int puedeCargar,string usuario){
	this->dibujarPantalla("boton.bmp");
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
	int cantidadCarga;
	string cantidadPlata("");
	//Para informar errores
	this->pantalla->dibujarRectangulo(0,this->infoconfig->alto*(0.9),this->infoconfig->ancho,24,255,255,255);
	if(carga){
		string titulo("Cargar hasta $");
		ostringstream sstream;
		sstream << puedeCargar;
		titulo += sstream.str();
		this->pantalla->dibujarRectangulo(8,55,144,20,255,255,255);
		this->pantalla->escribirTextoDesdePos(titulo.c_str(),10,10,40,blanco);
		this->pantalla->escribirStringDesdePos(cantidadPlata,13,50,25,0,0,0);
	}
	this->pantalla->dibujarRectangulo(8,165,144,40,255,255,255);
	this->pantalla->escribirTextoDesdePos("JUGAR",30,160,40,negro);
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
					if(evento.button.x>=8 and evento.button.x<=152 and evento.button.y>=165 and evento.button.y<=245){
						if(carga){
							cantidadCarga=atoi(cantidadPlata.c_str());
							if(cantidadCarga<=puedeCargar){
								if(cantidadPlata.compare("")==0) cantidadPlata="0";
								this->cargarFichas(cantidadPlata,usuario);
								return;
							} else {
								cantidadPlata.resize(0);
								this->pantalla->escribirTextoDesdePos("Supera el limite de 2000 fichas diarias",5,this->infoconfig->alto*(0.9),24,rojo);
								this->pantalla->dibujarRectangulo(8,55,144,20,200,200,200);
								this->actualizarPantalla();
								sleep(1);
								this->pantalla->dibujarRectangulo(8,55,144,20,255,255,255);
								this->pantalla->dibujarRectangulo(0,this->infoconfig->alto*(0.9),this->infoconfig->ancho,24,255,255,255);
								this->pantalla->escribirStringDesdePos(cantidadPlata,13,50,25,0,0,0);
								this->actualizarPantalla();
							}
						} else {
							return;
						}
					}
				}
			} else if(evento.type == SDL_KEYDOWN){
				if(evento.key.keysym.sym==SDLK_BACKSPACE and carga){
					int nuevoTamanio=cantidadPlata.size()-1;
					if (nuevoTamanio>=0){
						cantidadPlata.resize(cantidadPlata.size()-1);
						this->pantalla->dibujarRectangulo(8,55,144,20,255,255,255);
						this->pantalla->escribirStringDesdePos(cantidadPlata,13,50,25,0,0,0);
						this->actualizarPantalla();
					}
				} else if(evento.key.keysym.sym==SDLK_RETURN){
					if (carga){
						cantidadCarga=atoi(cantidadPlata.c_str());
						if(cantidadCarga<=puedeCargar){
							if(cantidadPlata.compare("")==0) cantidadPlata="0";
							this->cargarFichas(cantidadPlata,usuario);
							return;
						} else {
							cantidadPlata.resize(0);
							this->pantalla->escribirTextoDesdePos("Supera el limite de 2000 fichas diarias",5,this->infoconfig->alto*(0.9),24,rojo);
							this->pantalla->dibujarRectangulo(8,55,144,20,200,200,200);
							this->actualizarPantalla();
							sleep(1);
							this->pantalla->dibujarRectangulo(8,55,144,20,255,255,255);
							this->pantalla->dibujarRectangulo(0,this->infoconfig->alto*(0.9),this->infoconfig->ancho,24,255,255,255);
							this->pantalla->escribirStringDesdePos(cantidadPlata,13,50,25,0,0,0);
							this->actualizarPantalla();
						}
					} else {
						return;
					}
					//entre la a y la z o entre el 0 y el 9
				} else if((evento.key.keysym.sym>=48) and (evento.key.keysym.sym<=57) and carga){
					if(cantidadPlata.size()<4){
						caracterLeido=(char)evento.key.keysym.unicode;
						cantidadPlata+=caracterLeido;
						this->pantalla->dibujarRectangulo(8,55,144,20,255,255,255);
						this->pantalla->escribirStringDesdePos(cantidadPlata,13,50,25,0,0,0);
						this->actualizarPantalla();
					}
				}
			}
		}
	}
}

void Juego::dibujarPantallaRegistro(int cantidadIntentos){
//	this->pantalla->dibujarRectangulo(0,0,0,0,255,255,255);
	this->dibujarPantalla("boton.bmp");
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
		this->pantalla->escribirTextoDesdePos("El usuario ya existe o la ruta del archivo no es la correcta",5,this->infoconfig->alto*(0.9),24,rojo);
		this->actualizarPantalla();
	}
	this->pantalla->dibujarRectangulo(8,55,144,20,255,255,255);
	this->pantalla->dibujarRectangulo(8,165,144,20,255,255,255);
	this->pantalla->dibujarRectangulo(8,275,144,20,255,255,255);
	this->pantalla->dibujarRectangulo(8,365,80,30,255,255,255);
	this->pantalla->escribirTextoDesdePos("Usuario",10,10,40,blanco);
	this->pantalla->escribirTextoDesdePos("Contrasena",10,120,40,blanco);
	this->pantalla->escribirTextoDesdePos("Archivo",10,230,40,blanco);
	this->pantalla->escribirTextoDesdePos("Volver",10,355,40,negro);
	string usuarioTexto("");
	string contrasenaTexto("");
	string contrasenaAsteriscos("");
	string archivoTexto("");
	CasilleroTexto casillero=NINGUNO;
	this->pantalla->escribirStringDesdePos(usuarioTexto,13,50,25,0,0,0);
	this->pantalla->escribirStringDesdePos(contrasenaAsteriscos,13,160,25,0,0,0);
	this->pantalla->escribirStringDesdePos(archivoTexto,13,270,25,0,0,0);
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
							this->pantalla->escribirStringDesdePos(usuarioTexto,13,50,25,0,0,0);
							this->pantalla->escribirStringDesdePos(contrasenaAsteriscos,13,160,25,0,0,0);
							this->pantalla->escribirStringDesdePos(archivoTexto,13,270,25,0,0,0);
							casillero=USUARIO;
							this->actualizarPantalla();
						}
						else if(evento.button.y>=165 and evento.button.y<=185){
							this->pantalla->dibujarRectangulo(8,55,144,20,255,255,255);
							this->pantalla->dibujarRectangulo(8,165,144,20,200,200,200);
							this->pantalla->dibujarRectangulo(8,275,144,20,255,255,255);
							this->pantalla->escribirStringDesdePos(usuarioTexto,13,50,25,0,0,0);
							this->pantalla->escribirStringDesdePos(contrasenaAsteriscos,13,160,25,0,0,0);
							this->pantalla->escribirStringDesdePos(archivoTexto,13,270,25,0,0,0);
							casillero=CONTRASENA;
							this->actualizarPantalla();
						}
						else if(evento.button.y>=275 and evento.button.y<=295){
							this->pantalla->dibujarRectangulo(8,55,144,20,255,255,255);
							this->pantalla->dibujarRectangulo(8,165,144,20,255,255,255);
							this->pantalla->dibujarRectangulo(8,275,144,20,200,200,200);
							this->pantalla->escribirStringDesdePos(usuarioTexto,13,50,25,0,0,0);
							this->pantalla->escribirStringDesdePos(contrasenaAsteriscos,13,160,25,0,0,0);
							this->pantalla->escribirStringDesdePos(archivoTexto,13,270,25,0,0,0);
							casillero=ARCHIVO;
							this->actualizarPantalla();
						}
					} if(evento.button.x>=8 and evento.button.x<=88){
						if(evento.button.y>=365 and evento.button.y<=395){
							this->dibujarPantallaPrincipal();
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
							this->pantalla->escribirStringDesdePos(usuarioTexto,13,50,25,0,0,0);
							this->actualizarPantalla();
						}
					} else if(casillero==CONTRASENA){
						int nuevoTamanio=contrasenaTexto.size()-1;
						if (nuevoTamanio>=0){
							contrasenaTexto.resize(contrasenaTexto.size()-1);
							contrasenaAsteriscos.resize(contrasenaAsteriscos.size()-1);
							this->pantalla->dibujarRectangulo(8,165,144,20,200,200,200);
							this->pantalla->escribirStringDesdePos(contrasenaAsteriscos,13,160,25,0,0,0);
							this->actualizarPantalla();
						}
					} else if(casillero==ARCHIVO){
						int nuevoTamanio=archivoTexto.size()-1;
						if (nuevoTamanio>=0){
							archivoTexto.resize(archivoTexto.size()-1);
							this->pantalla->dibujarRectangulo(8,275,144,20,200,200,200);
							this->pantalla->escribirStringDesdePos(archivoTexto,13,270,25,0,0,0);
							this->actualizarPantalla();
						}
					}
				} else if(evento.key.keysym.sym==SDLK_RETURN){
					if(!this->registrarJugador(usuarioTexto,contrasenaTexto,archivoTexto)){
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
						this->pantalla->escribirStringDesdePos(usuarioTexto,13,50,25,0,0,0);
						this->actualizarPantalla();
					}
					if(casillero==CONTRASENA){
						contrasenaTexto+=caracterLeido;
						contrasenaAsteriscos+="*";
						this->pantalla->dibujarRectangulo(8,165,144,20,200,200,200);
						this->pantalla->escribirStringDesdePos(contrasenaAsteriscos,13,160,25,0,0,0);
						this->actualizarPantalla();
					}
					if(casillero==ARCHIVO){
						archivoTexto+=caracterLeido;
						this->pantalla->dibujarRectangulo(8,275,144,20,200,200,200);
						this->pantalla->escribirStringDesdePos(archivoTexto,13,270,25,0,0,0);
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

list<Carta>* Juego::cartasEnMesa(){
	return this->cartas;
}

bool Juego::esMiTurno(){
	string idOperacion = "A";
	list<string>* operandos = new list<string> ();
	char* xml = parser->getXmlDeOperacion(idOperacion, operandos);
	cliente->enviar(xml);
	char * respuesta = cliente->recibirRespuesta();
	string jugadorTurno = parserResultado->getPoso(respuesta);
	delete operandos;
	return (jugadorTurno==this->nombreJugador);
}

string Juego::getJugadorTurno(){
	string idOperacion = "A";
	list<string>* operandos = new list<string> ();
	char* xml = parser->getXmlDeOperacion(idOperacion, operandos);
	cliente->enviar(xml);
	char * respuesta = cliente->recibirRespuesta();
	string jugadorTurno = parserResultado->getPoso(respuesta);
	delete operandos;
	return jugadorTurno;
}

list<Jugador> Juego::getJugadores(){
	if(this->jugadores!=NULL)
		return *this->jugadores;
	list<Jugador> lista;
	return lista;
}

void Juego::mostrarYCargarDatos(int &iteracion, bool jugador_observador, bool jugador_virtual){
	SDL_Color blanco;
	blanco.r=255;
	blanco.g=255;
	blanco.b=255;
	string jugadorTurno(" ");
	string path(" ");
	this->cargarEscenario(this->escenario,true);
	list<Carta>* cartas = this->pedirCartas();
	cout << "hola " << endl;
	list<Jugador>* jugadores = this->pedirJugadores();
	this->pedirPoso();
	this->dibujarEscenario();
	this->pantalla->escribirTextoDesdePos("GANADOR: ",this->infoconfig->ancho - 300,10,40,blanco);
	list<Jugador>::iterator it = jugadores->begin();
	list<Carta>::iterator it2 = cartas->begin();
	while (it2 != cartas->end()) {
		this->dibujarCarta(*it2);
		it2++;
	}
	jugadorTurno=this->getJugadorTurno();
	while (it != jugadores->end()) {
		if(iteracion==5){
			path = this->pedirImagenJugador(&(*it));
		} else {
			string ruta = (*it).getNombre() + ".bmp";
			(*it).setPath(ruta);
		}
		this->pedirCartasJugador(&(*it));
		this->dibujarJugador(*it,jugadorTurno);
		it++;
	}
	this->pedirGanador();
	bool eraObservador=this->tipoJugador.jugadorObservador;
	if(this->hayGanador()){
		this->tipoJugador.jugadorObservador=true;
		this->pantalla->escribirTextoDesdePos(this->ganador.c_str(),this->infoconfig->ancho - 150,10,40,blanco);
	}else{
		this->pantalla->escribirTextoDesdePos(" ",this->infoconfig->ancho-100,10,20,blanco);
	}
	this->dibujarCartasJugadores();
	if(jugadores->size()<6){
		for(int i=(jugadores->size() + 1);i<7;i++){
			Jugador jugador("ImagenVacio.bmp"," "," ",i);
			this->dibujarJugador(jugador,jugadorTurno);
		}
	}
	if(cartas->size()<5){
		for(int i=(cartas->size()+1);i<6;i++){
			Carta carta("Imagen-Carta.bmp","Imagen","Carta",i);
			this->dibujarCarta(carta);
		}
	}
	this->dibujarPoso();
	if(!jugador_observador and !jugador_virtual){
		this->dibujarBoton("Pasar", 0);
		this->dibujarBoton("Igualar", 1);
		this->dibujarBoton("Apostar", 2);
		this->dibujarBoton("No Ir", 3);
	}
	else if(jugador_virtual){
		this->dibujarBoton("JUGAR",0);
	}
	this->pantalla->dibujarRectangulo(0,this->infoconfig->alto*(0.95),this->infoconfig->ancho,24,255,255,255);
	this->pantalla->escribirTextoDesdePos("SALIR",5,5,40,blanco);
	this->actualizarPantalla();
//	sleep(2);
	this->tipoJugador.jugadorObservador=eraObservador;
	if(iteracion==5){
		iteracion=0;
	} else {
		iteracion++;
	}
}

void Juego::jugar(bool jugador_observador, bool jugador_virtual){
	string path,pathEscenario;
	SDL_Color rojo;
	rojo.r=255;
	rojo.g=0;
	rojo.b=0;
	SDL_Color blanco;
	blanco.r=255;
	blanco.g=255;
	blanco.b=255;
	this->tipoJugador.jugadorObservador = jugador_observador;
	this->tipoJugador.jugadorVirtual = jugador_virtual;

	pthread_t thread;
	int create = pthread_create(&thread,NULL,manejoEventos,(void*)this);
	this->pedirEscenario();
	this->cargarEscenario(this->escenario,false);
	int iteracion=5;
	int tamfuente = this->infoconfig->ancho / 33;
	time_t tiempoInicial, tiempoActual;
	tiempoInicial;
	tiempoActual;
	double diferenciaTiempo;
	if(jugador_virtual){
		bool comenzoAJugar = false;
		this->jugadorVirtualAsignado = new JugadorVirtual(new Jugador("",this->nombreJugador,"",1));
	}
	while(true){
		this->enElTurno=this->esMiTurno();
		if(this->enElTurno){
			sleep(0.5);
			mostrarYCargarDatos(iteracion, jugador_observador, jugador_virtual);
			SDL_Color rojo;
			rojo.r = 255;
			rojo.g = 0;
			rojo.b = 0;
			SDL_Color blanco;
			blanco.r = 255;
			blanco.g = 255;
			blanco.b = 255;
			int inicio = this->infoconfig->ancho / 3.3;
			int distancia = this->infoconfig->ancho / 6;
			int factor = this->infoconfig->ancho / 42.5;
			int apuestaMax;
			string resultado;
			string idOperacion;
			int contadorOportunidades = 0;
			list<string>* operandos = new list<string> ();
			tiempoInicial =  time(NULL);
			idOperacion = "G";
			resultado = this->pedirOperacionDeJuego(idOperacion, operandos);
			apuestaMax = atoi(resultado.c_str());
			string apuesta = "Ultima Apuesta $: " + resultado;
			const char* apuestaC = apuesta.c_str();
			pantalla->escribirTextoDesdePos(apuestaC, this->infoconfig->ancho / 2.8 ,(this->infoconfig->alto / 54)+tamfuente,tamfuente,blanco);
			this->actualizarPantalla();
			//Hizo un ALL-IN por lo tanto pasa directamente
			if (this->plataJugador == 0) {
				idOperacion = "F";
				this->pedirOperacionDeJuego(idOperacion, operandos);
			}
			//No es un ALL-IN
			else{
			while (true) {
				SDL_Event evento;
				tiempoActual = time(NULL);
				diferenciaTiempo = difftime(tiempoActual, tiempoInicial);
				if(diferenciaTiempo < 15){
				if (SDL_PollEvent(&evento)) {
					if (evento.type == SDL_QUIT) {
						this->pantalla->escribirTextoDesdePos("CERRANDO",82,18,25,blanco);
						this->pantalla->actualizarPantalla(82,18,100,40);
						this->cerrar=true;
						break;
					} else if (evento.type == SDL_MOUSEBUTTONDOWN) {
						if (evento.button.button == 1) {
							if(evento.button.x < 80 and evento.button.y < 45){
								this->pantalla->escribirTextoDesdePos("CERRANDO",82,18,25,blanco);
								this->pantalla->actualizarPantalla(82,18,100,40);
								this->cerrar=true;
								break;
							} else if (!this->tipoJugador.jugadorVirtual) {
								idOperacion = "G";
								resultado = this->pedirOperacionDeJuego(idOperacion, operandos);
								apuestaMax = atoi(resultado.c_str());
								//Tiene 10 oportunidades para generar una opcion correcta
								if(contadorOportunidades < 10 ){
									if (evento.button.y>(this->infoconfig->alto/1.3) and evento.button.y<((this->infoconfig->alto/1.3)+this->infoconfig->alto/5.5)){
										if (evento.button.x>inicio and evento.button.x<inicio+distancia-factor){
											//PASAR
											if(apuestaMax==0){
												idOperacion = "F";
												if(!operandos->empty()) operandos->clear();
												this->pedirOperacionDeJuego(idOperacion, operandos);
												contadorOportunidades = 0;
												//Deja de ser mi turno
												break;
											} else{
												contadorOportunidades++;
												this->pantalla->escribirTextoDesdePos("No puede pasar, debe igualar, apostar o no ir", 5, this->infoconfig->alto * (0.95), 24, rojo);
												this->actualizarPantalla();
											}
										} else if (evento.button.x>inicio+distancia	and evento.button.x<inicio+2*distancia-factor){
											//IGUALAR
											if(this->plataJugador>=0){
												idOperacion="Y";
												if(!operandos->empty()) operandos->clear();
												this->pedirOperacionDeJuego(idOperacion, operandos);
												contadorOportunidades = 0;
												//Deja de ser mi turno
												break;
											}
										} else if (evento.button.x>inicio+2*distancia and evento.button.x<inicio+3*distancia-factor) {
											//APOSTAR
											//Ingresa primero cuanto quiere apostar
											bool aposto = false;
											string plataApuesta = "";
//											ostringstream sstream;
											int usuarioPlataApostada = 0;
											while (!aposto) {
													SDL_Event evento1;
													tiempoActual = time(NULL);
													diferenciaTiempo = difftime(tiempoActual, tiempoInicial);
													if(diferenciaTiempo < 15){
														if (SDL_PollEvent(&evento1)) {
															if(evento1.type == SDL_KEYDOWN){
																if(evento1.key.keysym.sym==SDLK_UP){
																	if(usuarioPlataApostada < this->plataJugador)
																		usuarioPlataApostada += 10;
																	ostringstream sstream;
																	sstream << usuarioPlataApostada;
																	plataApuesta = sstream.str();
																	this->pantalla->dibujarRectangulo(25,this->infoconfig->alto*0.825,60,25,200,200,200);
																	this->pantalla->escribirStringDesdePos(plataApuesta,25,this->infoconfig->alto*0.8,40,0,0,0);
																	this->actualizarPantalla();
																}else if(evento1.key.keysym.sym==SDLK_DOWN){
																	if(usuarioPlataApostada != 0)
																		usuarioPlataApostada -= 10;
																	ostringstream sstream;
																	sstream << usuarioPlataApostada;
																	plataApuesta = sstream.str();
																	this->pantalla->dibujarRectangulo(25,this->infoconfig->alto*0.825,60,25,200,200,200);
																	this->pantalla->escribirStringDesdePos(plataApuesta,25,this->infoconfig->alto*0.8,40,0,0,0);
																	this->actualizarPantalla();

															}
															else if((evento1.key.keysym.sym==SDLK_RETURN)){
																aposto = true;
																}
															}
														}
													}
													else{
														idOperacion = "D";
														operandos->push_front("Poso");
														operandos->push_back("0");
														this->pedirOperacionDeJuego(idOperacion,operandos);
														operandos->clear();
														contadorOportunidades = 0;
														//Deja de ser mi turno
														break;
													}
											}
											int plataNumero = usuarioPlataApostada;
											if(plataNumero <= this->plataJugador and plataNumero >= apuestaMax){
												idOperacion = "D";
												operandos->push_front("Poso");
												operandos->push_back(plataApuesta);
												this->pedirOperacionDeJuego(idOperacion, operandos);
												contadorOportunidades = 0;
												operandos->clear();
												//Deja de ser mi turno
												break;
											} else if(plataNumero < apuestaMax){
												contadorOportunidades++;
												this->pantalla->escribirTextoDesdePos("No puede apostar menos que la apuesta actual", 5, this->infoconfig->alto * (0.95), 24, rojo);
												this->actualizarPantalla();
											} else{
												contadorOportunidades++;
												this->pantalla->escribirTextoDesdePos("No puede apostar esa cantidad, no tiene fondos suficientes", 5, this->infoconfig->alto * (0.95), 24, rojo);
												this->actualizarPantalla();
											}
										} else if (evento.button.x>inicio+3*distancia and evento.button.x<inicio+4*distancia-factor){
											//NO IR
											idOperacion = "D";
											operandos->push_front("Poso");
											operandos->push_back("0");
											this->pedirOperacionDeJuego(idOperacion, operandos);
											operandos->clear();
											contadorOportunidades = 0;
											//Deja de ser mi turno
											break;
										}
									}
								}else{
									//Pasaron las 10 chances de realizar una operacion correcta por lo tanto es un NO IR
									idOperacion = "D";
									operandos->push_front("Poso");
									operandos->push_back("0");
									resultado = this->pedirOperacionDeJuego(idOperacion, operandos);
									operandos->clear();
									contadorOportunidades = 0;
								}
							} else {
								if (evento.button.y > (this->infoconfig->alto
										/ 1.3) and evento.button.y
										< ((this->infoconfig->alto / 1.3)
												+ this->infoconfig->alto / 5.5)) {
									if (evento.button.x > inicio
											and evento.button.x < inicio
													+ distancia - factor) {
										cout << "presiono boton JUGAR " << endl;
										idOperacion = "G";
										resultado = this->pedirOperacionDeJuego(idOperacion, operandos);
										apuestaMax = atoi(resultado.c_str());
										operandos = this->jugadorVirtualAsignado->decidirJugada(this->cartasJugador,this->cartasEnMesa(),this->plataJugador,apuestaMax);
										idOperacion = operandos->front();
										operandos->pop_front();
										resultado = this->pedirOperacionDeJuego(idOperacion, operandos);
										operandos->clear();
										//Deja de ser mi turno
										break;
									}
								}
							}
						}
					}
				}
				}
				else{
					idOperacion = "D";
					operandos->push_front("Poso");
					operandos->push_back("0");
					resultado = this->pedirOperacionDeJuego(idOperacion, operandos);
					operandos->clear();
					contadorOportunidades = 0;
					this->pantalla->escribirTextoDesdePos("Paso el tiempo de espera", 5, this->infoconfig->alto * (0.95), 24, rojo);
					this->actualizarPantalla();
					sleep(1);
					break;
				}
			}
			}
		} else {
			mostrarYCargarDatos(iteracion, jugador_observador, jugador_virtual);
		}
		if(this->cerrar) exit(0);
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

string Juego::pedirOperacionDeJuego(string idOperacion, list<string>* operandos) {
	char* xml = this->parser->getXmlDeOperacion(idOperacion, operandos);
	cliente->enviar(xml);
	char * respuesta = cliente->recibirRespuesta();
	string operacion = parserResultado->getPoso(respuesta);
	return operacion;
}
