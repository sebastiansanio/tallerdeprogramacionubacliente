#include "ParserCliente.h"

ParserCliente::ParserCliente() {
	// TODO Auto-generated constructor stub

}

ParserCliente::ParserCliente(const char* archivoXml){
	this->archivo = new ifstream(archivoXml);
	if(this->archivo->good()){
		this->fallido=false;
	} else {
		this->fallido=true;
	}
}

bool ParserCliente::comprobarSintaxis(){
	string* cadenaArchivo;
	string cadenaNodo;
	this->construirGrafo();
	list<Nodo*>* nodosActuales = (this->grafoTags->getNodoPorClave(0))->getNodosHijos();
	while(true){
		std::getline(*(this->archivo),*cadenaArchivo);
		list<Nodo*>::iterator iterador;
		iterador = nodosActuales->begin();
		while(iterador!=nodosActuales->end()){
			cadenaNodo=(*iterador)->getValor();
			if(this->comprobarTag(cadenaArchivo,&cadenaNodo)){
				break;
			}
			iterador++;
		}
		if(iterador!=nodosActuales->end()){
			//this->errorSintaxis(cadenaArchivo,&cadenaNodo);
			return false;
		}
		//Si no llegó al final de la lista es porque encontró el tag correcto y el iterador está parado en este
		nodosActuales = (*iterador)->getNodosHijos();
		//Si no tiene nodos hijos es porque se recorrió todo el grafo y la sintaxis está bien
		if(nodosActuales->empty()){
			return true;
		}
	}
}

bool ParserCliente::comprobarTag(string* cadenaArchivo,string* cadenaNodo){
	string bufferCadenaArchivo(*cadenaArchivo);
	string bufferCadenaNodo(*cadenaNodo);
	//saco lo que se encuentra entre los tag
	int ini = (int)bufferCadenaArchivo.find("<");
	int fin = (int)bufferCadenaArchivo.rfind(">");
	bufferCadenaArchivo = bufferCadenaArchivo.substr(ini+1,fin-ini-1);
	ini = (int)bufferCadenaNodo.find("<");
	fin = (int)bufferCadenaNodo.rfind(">");
	bufferCadenaNodo = bufferCadenaNodo.substr(ini+1,fin-ini-1);
	//comparo el nombre del tag
	int espacioArchivo = (int)bufferCadenaArchivo.find(" ");
	int espacioNodo = (int)bufferCadenaNodo.find(" ");
	if(bufferCadenaArchivo.compare(0,espacioArchivo,bufferCadenaNodo,0,espacioNodo)!=0){
		return false;
	}
	//comparo el atributo (funciona para un solo atributo)
	bufferCadenaArchivo = bufferCadenaArchivo.substr(0,bufferCadenaArchivo.find("="));
	bufferCadenaNodo = bufferCadenaNodo.substr(0,bufferCadenaNodo.find("="));
	espacioArchivo = (int)bufferCadenaArchivo.rfind(" ");
	espacioNodo = (int)bufferCadenaNodo.rfind(" ");
	if(bufferCadenaArchivo.compare(espacioArchivo+1,bufferCadenaArchivo.size()-espacioArchivo-1,bufferCadenaNodo,espacioNodo+1,bufferCadenaNodo.size()-espacioNodo-1)!=0){
		return false;
	}
	return true;
}

void ParserCliente::construirGrafo(){
	this->grafoTags = new Grafo();
	this->grafoTags->agregarNodo(new Nodo(0,"<pedido>"));
	this->grafoTags->agregarNodo(new Nodo(1,"<operacion id=\"\"/>"));
	this->grafoTags->agregarNodo(new Nodo(2,"<parametros>"));
	this->grafoTags->agregarNodo(new Nodo(3,"<parametro nombre=\"\">"));
	this->grafoTags->agregarNodo(new Nodo(4,"valor"));
	this->grafoTags->agregarNodo(new Nodo(5,"</parametro>"));
	this->grafoTags->agregarNodo(new Nodo(6,"</parametros>"));
	this->grafoTags->agregarNodo(new Nodo(7,"</pedido>"));
	this->grafoTags->agregarArista(0,1);
	this->grafoTags->agregarArista(1,2);
	this->grafoTags->agregarArista(2,3);
	this->grafoTags->agregarArista(3,4);
	this->grafoTags->agregarArista(4,5);
	this->grafoTags->agregarArista(5,3);
	this->grafoTags->agregarArista(5,6);
	this->grafoTags->agregarArista(6,7);
}

const char* ParserCliente::getSiguienteOperacion(){
	if(this->fallido==true){
		return "";
	}
	string* cadena = new string;
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
