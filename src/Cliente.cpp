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
	cout<<"Ingrese el id de la operacion:"<<endl;
	cout<<"S ----> Suma"<<endl;
	cout<<"R ----> Resta"<<endl;
	cout<<"M ----> Multiplicación"<<endl;
	cout<<"D ----> División"<<endl;
	cin>>idOperacion;
	string operando;
	string respuesta="S";
	while ((respuesta=="S")or(respuesta=="s")){
		list<string>::iterator i;
		i=operandos->begin();
		cout<<"Ingresar operando"<<endl;
		cin>>operando;
		operandos->insert(i,operando);
		cout<<"Desea ingresar otro operando? S/N"<<endl;
		cin>>respuesta;
		while((respuesta!="S")or(respuesta!="s")or(respuesta!="n")or(respuesta!="N")){
			cout<<"Desea ingresar otro operando? S/N"<<endl;
			cin>>respuesta;
		}
		i++;
	}
	this->parser->getXmlDeOperacion(idOperacion,operandos);
}

Cliente::~Cliente() {
	delete this->parser;
	delete this->parserResultado;
}
