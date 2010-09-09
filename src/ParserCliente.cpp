#include "ParserCliente.h"

ParserCliente::ParserCliente() {
	// TODO Auto-generated constructor stub

}

ParserCliente::ParserCliente(const char* archivoXml){
	this->archivo = new ifstream(archivoXml);
	//this->archivo->open(archivoXml, ios::in);
	this->construirGrafo();
	string* cadena = new string;
	if(this->archivo->good()){
		this->fallido=false;
	} else {
		this->fallido=true;
	}
	std::getline(*(this->archivo),*cadena);
	if((cadena->compare("<pedido>"))!=0)
		this->fallido=true;
	delete cadena;
}

void ParserCliente::construirGrafo(){
	this->grafoTags = new Grafo();
	Nodo* pedido = new Nodo(0,"pedido");
	Nodo* operacion = new Nodo(1,"operacion");
	Nodo* parametros = new Nodo(2,"parametros");
	Nodo* parametro = new Nodo(3,"parametro");
	Nodo* finparametro = new Nodo(4,"/parametro");
	Nodo* finparametros = new Nodo(5,"/parametros");
	Nodo* finpedido = new Nodo(6,"/pedido");
	pedido->agregarHijo(operacion);
	operacion->agregarHijo(parametros);
	parametros->agregarHijo(parametro);
	parametro->agregarHijo(finparametro);
	finparametro->agregarHijo(parametro);
	finparametro->agregarHijo(finparametros);
	finparametros->agregarHijo(finpedido);

	this->grafoTags->agregarNodo(pedido);
	this->grafoTags->agregarNodo(operacion);
	this->grafoTags->agregarNodo(parametros);
	this->grafoTags->agregarNodo(parametro);
	this->grafoTags->agregarNodo(finparametro);
	this->grafoTags->agregarNodo(finparametros);
	this->grafoTags->agregarNodo(finpedido);
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

string ParserCliente::devolverUnPedido(){
	char* lineaActual = new char;
	string pedidoCompleto = "";
	char* buffer;
	list<string>* errores = new list<string>();
	int numerofinpedido = 6;
	int numerolinea;
	Nodo* nodoAnterior = this->grafoTags->getNodoPorClave(0);
	Nodo* nodoActual = nodoAnterior;
	//Verifico si quedan lineas en el archivo y si se termino el pedido
	while(((*this->archivo) >> (lineaActual)) && (nodoActual->getClave()!= numerofinpedido)){
		//Aca va la logica con el grafo y las lineas del archivo
		//Es como lo que esta en el ParserServidor
		//buffer = strtok(lineaActual,"< >");


		pedidoCompleto  += (string)(lineaActual);
	}
	return pedidoCompleto;
}

ParserCliente::~ParserCliente() {
	this->archivo->close();
	delete this->archivo;
	delete this->archivoerrores;
	delete this->grafoTags;
}
