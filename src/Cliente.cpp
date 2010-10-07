#include "Cliente.h"

Cliente::Cliente() {
	this->parserResultado=new ParserResultadoCliente();
	this->parser= new ParserCliente();

	//creamos el socket
	descriptorSocket=socket(AF_INET,SOCK_STREAM,0);
	if(descriptorSocket==-1){
		cout<<"Mal creado socket del cliente"<<endl;
		cout<<"Se cerrará la aplicación"<<endl;
		sleep(2);
		exit(0);
	}

	//info del server
	estructuraDeDireccion.sin_family=AF_INET;//para lo de family QUE SEA LOCAL
	estructuraDeDireccion.sin_port=htons(PORT);
	estructuraDeDireccion.sin_addr.s_addr=INADDR_ANY;//INADDR_ANY coloca nuestra direccion IP automaticamente

	for(int i=0;i<8;i++){
			estructuraDeDireccion.sin_zero[i]=0;}
}

void Cliente::conectar(){
	socklen_t length=sizeof(sockaddr);

	//Conectar
	int valorConectar=connect(descriptorSocket,(struct sockaddr*)&estructuraDeDireccion,length);
	if(valorConectar==-1){
		cout<<"Mal conectado"<<endl;
		cout<<"Se cerrará la aplicación"<<endl;
		sleep(2);
		exit(0);
	}
}

void Cliente::enviar(char* data){
	ostringstream sstream;
	sstream << data;
	string paraVerCuantoPesa = sstream.str();
    unsigned int valorSend;
    valorSend = send(this->descriptorSocket, data, paraVerCuantoPesa.size(), 0);
	if(valorSend==0){
    	cout<<"Se desconecto el servidor.."<<endl;
    	cout<<"Se cerrará la aplicación"<<endl;
    	sleep(2);
    	exit(0);
	}
    if (valorSend == -1) { cout<<"Mal enviado"<<endl;}
    delete []data;
    char* data2=new char[3];
	memset((void*)data2,'\0',3);
    data2[0]='e';
	data2[1]='o';
	data2[2]='f';
	valorSend=send(this->descriptorSocket,data2,3,0);
	delete []data2;
}

void Cliente::interactuarConUsuarioYservidor(){

	system("clear");

	string idOperacion;
	list<string>* operandos=new list<string>();

	//Selección de Operacion
	cout<<"Ingrese el id de la operacion:"<<endl;
	cout<<"		S ----> Suma"<<endl;
	cout<<"		R ----> Resta"<<endl;
	cout<<"		M ----> Multiplicación"<<endl;
	cout<<"		D ----> División"<<endl;
	cin>>idOperacion;
	//Para pasar a mayuscula
	idOperacion=toupper(idOperacion[0]);

	while((idOperacion!="S")and(idOperacion!="R")and(idOperacion!="M")and(idOperacion!="D")){
		cout<<"Ingrese una operacion valida"<<endl;
		cin>>idOperacion;
		idOperacion=toupper(idOperacion[0]);
	}

	string nombreOperando;
	string valorOperando;
	string respuesta;

	//Ingreso de operandos
	switch(idOperacion[0]){
		case ('S'):nombreOperando="sum";respuesta="S";break;
		case ('R'):nombreOperando="res";respuesta="S";break;
		case ('M'):nombreOperando="mul";respuesta="S";break;
		case ('D'):respuesta="N";
					cout<<"Ingrese dividendo:"<<endl;
					cin>>valorOperando;
					operandos->push_back("dividendo");
					operandos->push_back(valorOperando);
					cout<<"Ingrese divisor:"<<endl;
					cin>>valorOperando;
					operandos->push_back("divisor");
					operandos->push_back(valorOperando);
					break;
	}

	//Lectura de operandos en caso que no se haya elegido una division
	while ((respuesta=="S")){
		cout<<"Ingresar operando"<<endl;
		cin>>valorOperando;
		operandos->push_back(nombreOperando);
		operandos->push_back(valorOperando);
		cout<<"Desea ingresar otro operando? S/N"<<endl;
		cin>>respuesta;
		respuesta=toupper(respuesta[0]);
		while((respuesta!="S")and(respuesta!="N")){
			cout<<"Desea ingresar otro operando? S/N"<<endl;
			cin>>respuesta;
			respuesta=toupper(respuesta[0]);
		}
	}
	//Generacion y envio de xml
	char* xml=this->parser->getXmlDeOperacion(idOperacion,operandos);
	enviar(xml);
	system("clear");
	cout<<"Mensaje enviado al Servidor"<<endl;
	cout<<endl;
	cout<<"Esperando Respuesta de la operación"<<endl;
	cout<<endl;
	this->recibir();
	sleep(2);
}

void Cliente::enviarArchivoOperaciones(string nombreArchivo){
	const char* nombreArchivoString = nombreArchivo.c_str();
	ParserCliente* parserArchivo= new ParserCliente(nombreArchivoString);
	int numeroOperacion=1;
	if((parserArchivo->comprobarSintaxis())==false){
		if(parserArchivo->huboErrorAlAbrirArchivo()){
			cout<<"Hubo un error al abrir el archivo, compuebe el nombre o dirección del mismo"<<endl;
			sleep(2);
		}else{
			cout<<"Error de sintaxis, ver archivo \"errores.err\""<<endl;
			sleep(2);}
	} else {
		char* xml = parserArchivo->getSiguienteOperacion();
		while(strcmp(xml,"")!=0){
			enviar(xml);
			system("clear");
			cout<<"Mensaje enviado al Servidor"<<endl;
			cout<<endl;
			cout<<"Esperando Respuesta de la operación nº: "<<numeroOperacion<<endl;
			cout<<endl;
			this->recibir();
			sleep(2);
			xml = parserArchivo->getSiguienteOperacion();
			numeroOperacion++;
		}
	}
	delete parserArchivo;
}

void Cliente::recibir(){
	char* data=new char[MAXBYTESRECIBIDOS];
	memset((void*)data,'\0',MAXBYTESRECIBIDOS);
	bool seguir=true;
	ofstream* archivoResultado = new ofstream("recibido", ios::out);
	socklen_t leng=sizeof(char[MAXBYTESRECIBIDOS]);
	ssize_t valorRecive;
	while(seguir){
		valorRecive=recv(this->descriptorSocket,data,leng,0);
		if(valorRecive==0){
       		cout<<"Se desconecto el servidor.."<<endl;
       		cout<<"Se cerrará la aplicación"<<endl;
       		sleep(2);
       		exit(0);
		}
		if(valorRecive==-1){
			cout<<"Mal recibido"<<endl;
		}else{
			//corroboro que los ultimos tres formen eof
			if((data[valorRecive-1]=='f')and(data[valorRecive-2]=='o')and(data[valorRecive-3]=='e')){
				seguir=false;
				ostringstream sstream;
				sstream << data;
				string lineaActual = sstream.str();
				memset((void*)data,'\0',MAXBYTESRECIBIDOS);
//				Para sacar el eof del archivo
				string::iterator it=lineaActual.end();
				it--;
				it=lineaActual.erase(it);
				it--;
				it=lineaActual.erase(it);
				it--;
				it=lineaActual.erase(it);
				*archivoResultado<<lineaActual;
			}else{
				ostringstream sstream;
				sstream << data;
				string lineaActual = sstream.str();
				*archivoResultado<<lineaActual;
				memset((void*)data,'\0',MAXBYTESRECIBIDOS);
				delete data;
				data=new char[MAXBYTESRECIBIDOS];
				memset((void*)data,'\0',MAXBYTESRECIBIDOS);
			}
		}
	}
	delete data;
	archivoResultado->close();
	delete archivoResultado;
	string recibido;
	string *recibidoAux=new string;
	ifstream* archivo=new ifstream("recibido");
	while(!archivo->eof()){
		std::getline(*archivo,*recibidoAux);
		recibido+=*recibidoAux;
		recibido+="\n";
	}
	char* dataAux=new char[recibido.size()];
	memset(dataAux,'\0',recibido.size());
	for(int i=0;i<recibido.size();i++) dataAux[i]=recibido[i];
	this->parserResultado->DecodificaResultado(dataAux);
	delete dataAux;
	archivo->close();

}
char* Cliente::recibirRespuesta(){
	char* data=new char[MAXBYTESRECIBIDOS];
	memset((void*)data,'\0',MAXBYTESRECIBIDOS);
	bool seguir=true;
	ofstream* archivoResultado = new ofstream("recibido", ios::out);
	socklen_t leng=sizeof(char[MAXBYTESRECIBIDOS]);
	ssize_t valorRecive;
	while(seguir){
		valorRecive=recv(this->descriptorSocket,data,leng,0);
		if(valorRecive==0){
       		cout<<"Se desconecto el servidor.."<<endl;
       		cout<<"Se cerrará la aplicación"<<endl;
       		sleep(2);
       		exit(0);
		}
		if(valorRecive==-1){
			cout<<"Mal recibido"<<endl;
		}else{
			//corroboro que los ultimos tres formen eof
			if((data[valorRecive-1]=='f')and(data[valorRecive-2]=='o')and(data[valorRecive-3]=='e')){
				seguir=false;
				ostringstream sstream;
				sstream << data;
				string lineaActual = sstream.str();
				memset((void*)data,'\0',MAXBYTESRECIBIDOS);
				//Para sacar el eof del archivo
				string::iterator it=lineaActual.end();
				it--;
				it=lineaActual.erase(it);
				it--;
				it=lineaActual.erase(it);
				it--;
				it=lineaActual.erase(it);
				*archivoResultado<<lineaActual;
			}else{
				ostringstream sstream;
				sstream << data;
				string lineaActual = sstream.str();
				*archivoResultado<<lineaActual;
				memset((void*)data,'\0',MAXBYTESRECIBIDOS);
				delete data;
				data=new char[MAXBYTESRECIBIDOS];
				memset((void*)data,'\0',MAXBYTESRECIBIDOS);
			}
		}
	}
	delete data;
	archivoResultado->close();
	delete archivoResultado;
	string recibido;
	string *recibidoAux=new string;
	ifstream* archivo=new ifstream("recibido");
	while(!archivo->eof()){
		std::getline(*archivo,*recibidoAux);
		recibido+=*recibidoAux;
		recibido+="\n";
	}
	char* dataAux=new char[recibido.size()];
	memset(dataAux,'\0',recibido.size());
	for(int i=0;i<recibido.size();i++) dataAux[i]=recibido[i];
	archivo->close();
	return dataAux;

}
string Cliente::recibirArchivo(string path){
	char* data=new char[MAXBYTESRECIBIDOS];
	memset((void*)data,'\0',MAXBYTESRECIBIDOS);
	bool seguir=true;
	ofstream* archivoResultado = new ofstream(path.c_str(), fstream::out | fstream::binary);
	socklen_t leng=sizeof(char[MAXBYTESRECIBIDOS]);
	ssize_t valorRecive;
	while(seguir){
		valorRecive=recv(this->descriptorSocket,data,leng,0);
		if(valorRecive==0){
       		cout<<"Se desconecto el servidor.."<<endl;
       		cout<<"Se cerrará la aplicación"<<endl;
       		sleep(2);
       		exit(0);
		}
		if(valorRecive==-1){
			cout<<"Mal recibido"<<endl;
		}else{
			//corroboro que los ultimos tres formen eof
			if((data[valorRecive-1]=='f')and(data[valorRecive-2]=='o')and(data[valorRecive-3]=='e')){
				seguir=false;
				int bytes=valorRecive - 3;
				char* final=new char[bytes];
				for(int i=0;i<(valorRecive-3);i++) final[i]=data[i];
				archivoResultado->write(final,bytes);
				memset((void*)data,'\0',MAXBYTESRECIBIDOS);
			}else{
				archivoResultado->write(data,valorRecive);
				memset((void*)data,'\0',MAXBYTESRECIBIDOS);
				delete []data;
				data=new char[MAXBYTESRECIBIDOS];
				memset((void*)data,'\0',MAXBYTESRECIBIDOS);
			}
		}
	}
	delete []data;
	archivoResultado->close();
	delete archivoResultado;
	return path;
}
Cliente::~Cliente() {
	delete this->parser;
	delete this->parserResultado;
}
