#include "ParserCliente.h"

ParserCliente::ParserCliente() {
	// TODO Auto-generated constructor stub

}

ParserCliente::ParserCliente(const char* archivoXml){
	this->archivo = new ifstream(archivoXml);
}

const char* ParserCliente::getSiguienteOperacion(){

}

const char* ParserCliente::getXmlDeOperacion(string idOperacion, list<string>* operandos){
	string* aEnviar = new string;
	(*aEnviar)="<pedido>\n	<operacion id=\""+idOperacion+"\"/>\n	<parametros>\n";
	list<string>::const_iterator iterador;
	iterador=operandos->begin();
	while(iterador!=operandos->end()){
		(*aEnviar)+="		<parametro nombre=\""+(*iterador)+"\">\n";
		iterador++;
		(*aEnviar)+="		"+(*iterador)+"\n		</parametro>\n";
		iterador++;
	}
	(*aEnviar)+="	</parametros>\n</pedido>";
	return (aEnviar->c_str());
}

ParserCliente::~ParserCliente() {
	this->archivo->close();
}
