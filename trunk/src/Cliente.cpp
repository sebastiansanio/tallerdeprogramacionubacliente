#include "Cliente.h"

Cliente::Cliente() {
	this->parserResultado=new ParserResultadoCliente();
	this->parser= new ParserCliente();
}

void Cliente::onConnect(){
	cout<<"Conectado Correctamente"<<endl;
}

void Cliente::onDataArrival(string Data){
	cout<<"Decodificando Respuesta..."<<endl;
	this->parserResultado->DecodificaResultado(Data);
}

void Cliente::enviarOperacion(){
	system("cls");
	string idOperacion;
	list<string>* operandos=new list<string>();
	//Selección de Operacion
	cout<<"Ingrese el id de la operacion:"<<endl;
	cout<<"		S ----> Suma"<<endl;
	cout<<"		R ----> Resta"<<endl;
	cout<<"		M ----> Multiplicación"<<endl;
	cout<<"		D ----> División"<<endl;
	cin>>idOperacion;
	while((idOperacion!="S")or(idOperacion!="R")or(idOperacion!="M")or(idOperacion!="D")or(idOperacion!="s")or(idOperacion!="r")or(idOperacion!="m")or(idOperacion!="d")){
		cout<<"Ingrese una operacion valida"<<endl;
		cin>>idOperacion;
	}
	string nombreOperando;
	string valorOperando;
	string respuesta;

	//Ingreso de operandos
	switch(idOperacion[0]){
		case ('S','s'):{nombreOperando="sum";break;respuesta="S";}
		case ('R','r'):{nombreOperando="res";break;respuesta="S";}
		case ('M','m'):{nombreOperando="mul";break;respuesta="S";}
		case ('D','d'):{respuesta="N";
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
						}
	}
	while ((respuesta=="S")or(respuesta=="s")){
		list<string>::iterator i;
		i=operandos->begin();
		cout<<"Ingresar operando"<<endl;
		cin>>valorOperando;
		operandos->insert(i,nombreOperando);
		i++;
		operandos->insert(i,valorOperando);
		cout<<"Desea ingresar otro operando? S/N"<<endl;
		cin>>respuesta;
		while((respuesta!="S")or(respuesta!="s")or(respuesta!="n")or(respuesta!="N")){
			cout<<"Desea ingresar otro operando? S/N"<<endl;
			cin>>respuesta;
		}
		i++;
	}
	string xml=this->parser->getXmlDeOperacion(idOperacion,operandos);
	Send(xml);
	cout<<"Mensaje enviado al Servidor"<<endl;
}

Cliente::~Cliente() {
	delete this->parser;
	delete this->parserResultado;
}
