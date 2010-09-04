#include "Cliente.h"

Cliente::Cliente() {
	this->parserResultado=new ParserResultadoCliente();
	this->parser= new ParserCliente();

	//creamos el socket
	descriptorSocket=socket(AF_INET,SOCK_STREAM,0);
	if(descriptorSocket==-1){
		cout<<"Mal creado socket"<<endl;
	}
	else{
		cout<<"Socket bien creado"<<endl;
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
	}else{
		cout<<"Conectado"<<endl;
	}
	fcntl(descriptorSocket, F_SETFL, O_NONBLOCK);
}

void Cliente::enviar(char data[]){
	size_t leng=sizeof(char[MAXBYTES]);
	socklen_t length=sizeof(sockaddr);
	int valorSend=send(descriptorSocket,data,leng,0);
	if(valorSend==-1){
		cout<<"Mal enviado"<<endl;
	}else{
		cout<<"Se envio info"<<endl;
	}
}

void Cliente::enviarOperacion(){

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

	while((idOperacion!="S")or(idOperacion!="R")or(idOperacion!="M")or(idOperacion!="D")){
		cout<<"Ingrese una operacion valida"<<endl;
		cin>>idOperacion;
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
					list<string>::iterator i;
					i=operandos->begin();
					cout<<"Ingrese dividendo:"<<endl;
					cin>>valorOperando;
					operandos->insert(i,"dividendo");
					i++;
					operandos->insert(i,valorOperando);
					cout<<"Ingrese divisor:"<<endl;
					cin>>valorOperando;
					operandos->insert(i,"divisor");
					i++;
					operandos->insert(i,valorOperando);
					break;
	}

	//Lectura de operandos en caso que no se haya elegido una division
	while ((respuesta=="S")){
		list<string>::iterator i;
		i=operandos->begin();
		cout<<"Ingresar operando"<<endl;
		cin>>valorOperando;
		operandos->insert(i,nombreOperando);
		i++;
		operandos->insert(i,valorOperando);
		cout<<"Desea ingresar otro operando? S/N"<<endl;
		cin>>respuesta;
		while((respuesta!="S")and(respuesta!="s")and(respuesta!="n")and(respuesta!="N")){
			cout<<"Desea ingresar otro operando? S/N"<<endl;
			cin>>respuesta;
			respuesta=toupper(respuesta[0]);
		}
		i++;
	}

	//Generacion y envio de xml
	char* xml=this->parser->getXmlDeOperacion(idOperacion,operandos);
	enviar(xml);
	cout<<"Mensaje enviado al Servidor"<<endl;
}
void Cliente::recibir(){
	char data[MAXBYTES];
	socklen_t leng=sizeof(data);
	ssize_t valorRecive=recv(this->descriptorSocket,&data,leng,0);
	if(valorRecive==-1){
		cout<<"Mal recibido"<<endl;
	}else{
		data[valorRecive]='\0';
		cout<<data<<endl;
	}
}
Cliente::~Cliente() {
	delete this->parser;
	delete this->parserResultado;
}
