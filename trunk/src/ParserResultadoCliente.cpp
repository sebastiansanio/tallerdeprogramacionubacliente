#include "ParserResultadoCliente.h"

ParserResultadoCliente::ParserResultadoCliente() {
	// TODO Auto-generated constructor stub

}

bool ParserResultadoCliente::DecodificaResultado(char xml []){
	bool resultadoOk;
	ostringstream sstream;
	sstream << xml;
	string paraVerCuantoPesa = sstream.str();
	char xmlAux[paraVerCuantoPesa.size()];
	char xmlAux2[paraVerCuantoPesa.size()];
	for(unsigned int i=0;i<paraVerCuantoPesa.size();i++){xmlAux[i]=xml[i];xmlAux2[i]=xml[i];}
	char* buffer = strtok(xml,"\n\t<>");
	buffer = strtok(NULL,"\n\t<>");
	string operacion = buffer;
	buffer = strtok(NULL,"\n\t<>");
	string tipo;
	char* palabraclave;

	//Registro el resultado en el archivo correspondiente
	if (strcmp(buffer, "resultados") == 0) {
		tipo = "El resultado de ";
		palabraclave = "nombre";
		this->registrarResultado(xmlAux,operacion,"resultados","resultados");
		resultadoOk=true;
	} else {
		tipo = "El error de tipo ";
		palabraclave = "tipo";
		this->registrarResultado(xmlAux,operacion,"errores","errores.err");
		resultadoOk=false;
	}
	//Reinicio el buffer para imprimir por pantalla los errores o resultados
	buffer = strtok(xmlAux2,"<>");
	buffer = strtok(NULL,"<>");
	buffer = strtok(NULL,"<>");

	while (buffer != NULL) {
		if (strcmp(buffer, palabraclave) == 0) {
			buffer = strtok(NULL, "\t <>=\"");
			cout << tipo << buffer << " es: ";
			buffer = strtok(NULL, ">\n\t");
			cout << buffer << endl;
		}
		buffer = strtok(NULL, " \t<>=");
	}
	return resultadoOk;
}

void ParserResultadoCliente::registrarResultado(char xml [],string operacion, string tipo, const char* archivo){
	(this->archivoResultado) = new ofstream(archivo, ios::out | ios::app);
	string lineaActual;
	lineaActual=xml;
	*this->archivoResultado << "<respuesta>" << endl;
	*this->archivoResultado << "\t<" << operacion << ">" << endl;
	*this->archivoResultado << "\t\t<" << tipo << ">" << endl;
	lineaActual = strtok(NULL, "<>\t\n");
	string auxiliar="/" + tipo;
	while (lineaActual != auxiliar) {
		*this->archivoResultado << "\t\t\t<" << lineaActual << ">" << endl;
		lineaActual = strtok(NULL, "<>\t\n");
		*this->archivoResultado << "\t\t\t" << lineaActual << endl;
		lineaActual = strtok(NULL, "<>\t\n");
		*this->archivoResultado << "\t\t\t<" << lineaActual << ">" << endl;
		lineaActual = strtok(NULL, "<>\t\n");
	}
	*this->archivoResultado << "\t\t</" << tipo << ">" << endl;
	*this->archivoResultado << "</respuesta>" << endl;
	this->archivoResultado->close();
	delete this->archivoResultado;

}

list<string> * ParserResultadoCliente::getJugadores(char xml[]){
	ostringstream sstream;
	sstream << xml;
	string paraVerCuantoPesa = sstream.str();
	char xmlAux[paraVerCuantoPesa.size()];
	char xmlAux2[paraVerCuantoPesa.size()];
	for (unsigned int i = 0; i < paraVerCuantoPesa.size(); i++) {
		xmlAux[i] = xml[i];
		xmlAux2[i] = xml[i];
	}
	char* buffer = strtok(xml, "\n\t<>");
	buffer = strtok(NULL, "\n\t<>");
	string operacion = buffer;
	buffer = strtok(NULL, "\n\t<>");
	string tipo;
	char* palabraclave;

	//Registro el resultado en el archivo correspondiente
	if (strcmp(buffer, "resultados") == 0) {
		tipo = "El nombre del jugador es: ";
		palabraclave = "nombre";
		this->registrarResultado(xmlAux, operacion, "resultados", "resultados");
	} else {
		tipo = "El error de tipo ";
		palabraclave = "tipo";
		this->registrarResultado(xmlAux, operacion, "errores", "errores.err");
	}
	//Reinicio el buffer
	buffer = strtok(xmlAux2, "<>");
	buffer = strtok(NULL, "<>");
	buffer = strtok(NULL, "<>");
	list<string>* respuesta = new list<string>();
	list<string>::iterator it;
	it = respuesta->begin();
	while (buffer != NULL) {
		if (strcmp(buffer, palabraclave) == 0) {
			buffer = strtok(NULL, "\t <>=\"");
//			cout << tipo << buffer << " y su plata: ";
			it = respuesta->insert(it, buffer);
			it++;
			buffer = strtok(NULL, ">\n\t");
//			cout << buffer << endl;
			it = respuesta->insert(it, buffer);
			it++;
		}
		buffer = strtok(NULL, " \t<>=");
	}
	return respuesta;
}

bool ParserResultadoCliente::getPlataYCargado(char xml[],int* plata, int* cargado){
	bool resultadoOk;
	ostringstream sstream;
	sstream << xml;
	string paraVerCuantoPesa = sstream.str();
	char xmlAux[paraVerCuantoPesa.size()];
	char xmlAux2[paraVerCuantoPesa.size()];
	for(unsigned int i=0;i<paraVerCuantoPesa.size();i++){xmlAux[i]=xml[i];xmlAux2[i]=xml[i];}
	char* buffer = strtok(xml,"\n\t<>");
	buffer = strtok(NULL,"\n\t<>");
	string operacion = buffer;
	buffer = strtok(NULL,"\n\t<>");
	string tipo;
	char* palabraclave;

	//Registro el resultado en el archivo correspondiente
	if (strcmp(buffer, "resultados") == 0) {
		tipo = "El resultado de ";
		palabraclave = "nombre";
		this->registrarResultado(xmlAux,operacion,"resultados","resultados");
		resultadoOk=true;
	} else {
		tipo = "El error de tipo ";
		palabraclave = "tipo";
		this->registrarResultado(xmlAux,operacion,"errores","errores.err");
		resultadoOk=false;
	}
	//Reinicio el buffer para imprimir por pantalla los errores o resultados
	buffer = strtok(xmlAux2,"<>");
	buffer = strtok(NULL,"<>");
	buffer = strtok(NULL,"<>");
	int contador=0;
	while (buffer != NULL) {
		if (strcmp(buffer, palabraclave) == 0) {
			buffer = strtok(NULL, "\t <>=\"");
			cout << tipo << buffer << " es: ";
			buffer = strtok(NULL, ">\n\t");
			cout << buffer << endl;
			if(contador==2 and resultadoOk) (*plata)=atoi(buffer);
			if(contador==3 and resultadoOk) (*cargado)=atoi(buffer);
			contador++;
		}
		buffer = strtok(NULL, " \t<>=");
	}
	return resultadoOk;
}

list<string> * ParserResultadoCliente::getCartas(char xml[]) {
	ostringstream sstream;
	sstream << xml;
	string paraVerCuantoPesa = sstream.str();
	char xmlAux[paraVerCuantoPesa.size()];
	char xmlAux2[paraVerCuantoPesa.size()];
	for (unsigned int i = 0; i < paraVerCuantoPesa.size(); i++) {
		xmlAux[i] = xml[i];
		xmlAux2[i] = xml[i];
	}
	char* buffer = strtok(xml, "\n\t<>");
	buffer = strtok(NULL, "\n\t<>");
	string operacion = buffer;
	buffer = strtok(NULL, "\n\t<>");
	string tipo;
	char* palabraclave;

	//Registro el resultado en el archivo correspondiente
	if (strcmp(buffer, "resultados") == 0) {
		tipo = "El palo de la carta es: ";
		palabraclave = "nombre";
		this->registrarResultado(xmlAux, operacion, "resultados", "resultados");
	} else {
		tipo = "El error de tipo ";
		palabraclave = "tipo";
		this->registrarResultado(xmlAux, operacion, "errores", "errores.err");
		return NULL;
	}
	//Reinicio el buffer
	buffer = strtok(xmlAux2, "<>");
	buffer = strtok(NULL, "<>");
	buffer = strtok(NULL, "<>");
	list<string>* respuesta = new list<string> ();
	list<string>::iterator it;
	it = respuesta->begin();
	while (buffer != NULL) {
		if (strcmp(buffer, palabraclave) == 0) {
			buffer = strtok(NULL, "\t <>=\"");
//			cout << tipo << buffer << " y el numero es: ";
			it = respuesta->insert(it, buffer);
			it++;
			buffer = strtok(NULL, ">\n\t");
//			cout << buffer << endl;
			it = respuesta->insert(it, buffer);
			it++;
		}
		buffer = strtok(NULL, " \t<>=");
	}
	return respuesta;
}

string ParserResultadoCliente::getPoso(char xml[]) {
	ostringstream sstream;
	sstream << xml;
	string paraVerCuantoPesa = sstream.str();
	char xmlAux[paraVerCuantoPesa.size()];
	char xmlAux2[paraVerCuantoPesa.size()];
	for (unsigned int i = 0; i < paraVerCuantoPesa.size(); i++) {
		xmlAux[i] = xml[i];
		xmlAux2[i] = xml[i];
	}
	char* buffer = strtok(xml, "\n\t<>");
	buffer = strtok(NULL, "\n\t<>");
	string operacion = buffer;
	buffer = strtok(NULL, "\n\t<>");
	string tipo;
	char* palabraclave;

	//Registro el resultado en el archivo correspondiente
	if (strcmp(buffer, "resultados") == 0) {
		tipo = "El ";
		palabraclave = "nombre";
		this->registrarResultado(xmlAux, operacion, "resultados", "resultados");
	} else {
		tipo = "El error de tipo ";
		palabraclave = "tipo";
		this->registrarResultado(xmlAux, operacion, "errores", "errores.err");
	}
	//Reinicio el buffer
	buffer = strtok(xmlAux2, "<>");
	buffer = strtok(NULL, "<>");
	buffer = strtok(NULL, "<>");
	string resultado;
	//Solo en el caso de que sea un archivo de resultados va a tener sentido lo que contenga resultado
	while (buffer != NULL) {
		if (strcmp(buffer, palabraclave) == 0) {
			buffer = strtok(NULL, "\t <>=\"");
//			cout << tipo << buffer << " es: ";
			buffer = strtok(NULL, ">\n\t");
//			cout << buffer << endl;
			resultado = buffer;
		}
		buffer = strtok(NULL, " \t<>=");
	}
	return resultado;
}


ParserResultadoCliente::~ParserResultadoCliente() {
}
