#include "ParserCliente.h"

ParserCliente::ParserCliente() {
	// TODO Auto-generated constructor stub

}

ParserCliente::ParserCliente(const char* archivoXml){
	this->archivo = new ifstream(archivoXml);
}


char* ParserCliente::getXmlDeOperacion(string idOperacion, list<string>* operandos){
	char* aEnviar=new char[300];
	return aEnviar;
}

ParserCliente::~ParserCliente() {
	this->archivo->close();
}
