#include "ParserCliente.h"

ParserCliente::ParserCliente() {
	tieneArchivo=false;
}

ParserCliente::ParserCliente(const char* archivoXml){
	this->archivo = new ifstream(archivoXml);
	if(this->archivo->good()){
		this->fallido=false;
		tieneArchivo=true;
	} else {
		this->fallido=true;
	}
}

bool ParserCliente::comprobarSintaxis(){
	if(this->fallido){
		//ver que hacer cuando hay un error al abrir el archivo
	}
	string* cadenaArchivo = new string;
	//Para el error
	string operacionid("");
	string error;
	list<string>* listaErrores = new list<string>;
	char contador[10];
	int contadorLinea=0;
	string cadenaNodo;
	bool tagCorrecto;
	this->construirGrafo();
	list<Nodo*>* nodosActuales = (this->grafoTags->getNodoPorClave(0))->getNodosHijos();
	while(true){
		std::getline(*(this->archivo),*cadenaArchivo);
		//Si termino el archivo me fijo si estoy en el eof del grafo
		contadorLinea++;
		list<Nodo*>::iterator iterador;
		iterador = nodosActuales->begin();
		if(this->archivo->eof()==true){
			while(iterador!=nodosActuales->end()){
				cadenaNodo=(*iterador)->getValor();
				if(cadenaNodo.compare("eof")==0){
					return true;
				}
				iterador++;
			}
			return false;
		}
		tagCorrecto=false;
		while((iterador!=nodosActuales->end()) and (!tagCorrecto)){
			cadenaNodo=(*iterador)->getValor();
			if(this->comprobarTag(cadenaArchivo,&cadenaNodo)){
				tagCorrecto=true;
			} else {
				iterador++;
			}
		}
		if(!tagCorrecto){
			error+=("La linea ");
			sprintf(contador,"%d",contadorLinea);
			error+=contador;
			error+=" muestra: ";
			error+=*cadenaArchivo;
			error+=" debe mostrar: ";
			error+=cadenaNodo;
			listaErrores->push_back("S");
			listaErrores->push_back(error);
			this->registrarError(operacionid,listaErrores);
			return false;
		}
		nodosActuales = (*iterador)->getNodosHijos();
		//Si no llegó al final de la lista es porque encontró el tag correcto y el iterador está parado en este
		//Si no tiene nodos hijos es porque se recorrió todo el grafo y la sintaxis está bien
		if(nodosActuales->empty()){
			return true;
		}
	}
}

bool ParserCliente::comprobarTag(string* cadenaArchivo,string* cadenaNodo){
	if(cadenaNodo->compare("valor")==0){
		return true;
	}
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
	//Ahora analiza la sintaxis de otra forma, validando otras cuestiones
	const char* charArchivo = cadenaArchivo->c_str();
	const char* charNodo = cadenaNodo->c_str();
	int tamanioArchivo=(int)strlen(charArchivo);
	int tamanioNodo=(int)strlen(charNodo);
	int tamanio;
	if(tamanioArchivo<tamanioNodo){
		tamanio=tamanioNodo;
	} else {
		tamanio=tamanioArchivo;
	}
	char bufferCharArchivo[tamanio];
	char bufferCharNodo[tamanio];
	int posicion=0;
	bool hayComilla=false;
	for(int i=0;i<tamanioArchivo+1;i++){
		if((charArchivo[i]!=' ') and (charArchivo[i]!='	') and (!hayComilla)){
			bufferCharArchivo[posicion]=charArchivo[i];
			posicion++;
		}
		if(charArchivo[i]=='\"'){
			if(hayComilla){
				hayComilla=false;
				bufferCharArchivo[posicion]=charArchivo[i];
				posicion++;
			} else {
				hayComilla=true;
			}
		}
	}
	posicion=0;
	hayComilla=false;
	for(int i=0;i<tamanioNodo+1;i++){
		if((charNodo[i]!=' ') and (charNodo[i]!='	') and (!hayComilla)){
			bufferCharNodo[posicion]=charNodo[i];
			posicion++;
		}
		if(charNodo[i]=='\"'){
			if(hayComilla){
				hayComilla=false;
				bufferCharNodo[posicion]=charNodo[i];
				posicion++;
			} else {
				hayComilla=true;
			}
		}
	}
	return(strcmp(bufferCharArchivo,bufferCharNodo)==0);
}

void ParserCliente::construirGrafo(){
	this->grafoTags = new Grafo();
	this->grafoTags->agregarNodo(new Nodo(0,"raiz"));
	this->grafoTags->agregarNodo(new Nodo(1,"<pedido>"));
	this->grafoTags->agregarNodo(new Nodo(2,"<operacion id=\"\"/>"));
	this->grafoTags->agregarNodo(new Nodo(3,"<parametros>"));
	this->grafoTags->agregarNodo(new Nodo(4,"<parametro nombre=\"\">"));
	this->grafoTags->agregarNodo(new Nodo(5,"valor"));
	this->grafoTags->agregarNodo(new Nodo(6,"</parametro>"));
	this->grafoTags->agregarNodo(new Nodo(7,"</parametros>"));
	this->grafoTags->agregarNodo(new Nodo(8,"</pedido>"));
	this->grafoTags->agregarNodo(new Nodo(9,""));
	this->grafoTags->agregarArista(0,1);
	this->grafoTags->agregarArista(1,2);
	this->grafoTags->agregarArista(2,3);
	this->grafoTags->agregarArista(3,4);
	this->grafoTags->agregarArista(4,5);
	this->grafoTags->agregarArista(5,6);
	this->grafoTags->agregarArista(6,4);
	this->grafoTags->agregarArista(6,7);
	this->grafoTags->agregarArista(7,8);
	this->grafoTags->agregarArista(8,1);
	this->grafoTags->agregarArista(8,9);
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
	(this->archivoerrores) = new ofstream("errores.err", ios::out | ios::app);
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
	if(this->tieneArchivo){
		this->archivo->close();
		delete this->archivo;
	}
	delete this->archivoerrores;
	delete this->grafoTags;
}
