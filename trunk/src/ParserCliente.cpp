#include "ParserCliente.h"

ParserCliente::ParserCliente() {
	// TODO Auto-generated constructor stub

}

ParserCliente::ParserCliente(const char* archivoXml){
	this->archivo = new ifstream(archivoXml);

	string* cadena = new string;
	if(this->archivo->good()){
		this->fallido=false;
	} else {
		this->fallido=true;
	}
	std::getline(*(this->archivo),*cadena);
	if((cadena->compare("<pedido>"))!=0)this->fallido=true;
	delete cadena;
}



const char* ParserCliente::getSiguienteOperacion(){
	if(this->fallido=true){
		return "";
	}
	string* cadena = new string;

	if(this->fallido=true){
			return "";
	}
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

void ParserCliente::registrarError(string idOperacion, list<string>* mensajesError) {
	(*this->archivoerrores).open("errores.err", ios::out | ios::app);
	(*this->archivoerrores) << "<respuesta>" << endl;
	(*this->archivoerrores) << "\t<operacion id=\"" << idOperacion << "\"/>" << endl;
	(*this->archivoerrores) << "\t<errores>" << endl;
	list<string>::const_iterator iterador;
	iterador = mensajesError->begin();
	while(iterador!=mensajesError->end()){
		//La lista tiene primero el tipo de error y luego sigue el mensaje, pueden haber varios
		(*this->archivoerrores) << "\t\t<error tipo=\"" << (*iterador) << "\">" << endl;
		iterador++;
		(*this->archivoerrores) << "\t\t" << (*iterador) << endl;
		iterador++;
		(*this->archivoerrores) << "\t\t</error>" << endl;
	}
	(*this->archivoerrores) << "\t</errores>" << endl;
	(*this->archivoerrores) << "</respuesta>" << endl;

	(*this->archivoerrores).close();
}

ParserCliente::~ParserCliente() {
	this->archivo->close();
	delete this->archivo;
	delete this->archivoerrores;
}
