#include "Cliente.h"

Cliente::Cliente() {
        //creamos el socket
        descriptorSocket=socket(AF_INET,SOCK_STREAM,0);
        if(descriptorSocket==-1){
                cout<<"Mal creado socket del cliente"<<endl;
                cout<<"Se cerrará la aplicación"<<endl;
                sleep(2);
                exit(0);
        }

        //info del server
        estructuraDeDireccion.sin_family=AF_INET;//para lo de family QUE SEA LOCAL
        estructuraDeDireccion.sin_port=htons(PORT);
        estructuraDeDireccion.sin_addr.s_addr=INADDR_ANY;//INADDR_ANY coloca nuestra direccion IP automaticamente

        for(int i=0;i<8;i++){
                        estructuraDeDireccion.sin_zero[i]=0;}
}

Cliente::Cliente(unsigned int puerto, string IP){
	//creamos el socket
	descriptorSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (descriptorSocket == -1) {
		cout << "Mal creado socket del cliente" << endl;
		cout << "Se cerrará la aplicación" << endl;
		sleep(2);
		exit(0);
	}

	//info del server
	estructuraDeDireccion.sin_family = AF_INET;//para lo de family QUE SEA LOCAL
	estructuraDeDireccion.sin_port = htons(puerto);
	estructuraDeDireccion.sin_addr.s_addr = inet_addr(IP.c_str());//INADDR_ANY coloca nuestra direccion IP automaticamente

	for (int i = 0; i < 8; i++) {
		estructuraDeDireccion.sin_zero[i] = 0;
	}


}

bool Cliente::conectar(){
        socklen_t length=sizeof(sockaddr);

        //Conectar
        int valorConectar=connect(descriptorSocket,(struct sockaddr*)&estructuraDeDireccion,length);
        return (valorConectar!=-1);
}

void Cliente::enviar(char* data){
    ostringstream sstream;
    sstream << data;
    string paraVerCuantoPesa = sstream.str();
    unsigned int valorSend;
    valorSend = send(this->descriptorSocket, data, paraVerCuantoPesa.size(), 0);
    if(valorSend==0){
        cout<<"Se desconecto el servidor.."<<endl;
        cout<<"Se cerrará la aplicación"<<endl;
        sleep(2);
        exit(0);
    }
    if (valorSend == -1) { cout<<"Mal enviado"<<endl;}
    delete []data;
    char* data2=new char[3];
    memset((void*)data2,'\0',3);
    data2[0]='e';
    data2[1]='o';
    data2[2]='f';
    valorSend=send(this->descriptorSocket,data2,3,0);
    delete []data2;
}

int Cliente::enviarArchivoBMP(string path){
    // referencia al archivo
    fstream  archivo;
    //intenta abrir el archivo en modo lectura y binario
    archivo.open(path.c_str(), fstream::in | fstream::binary );
    if(!archivo.good()){
    	cout<<"Mal imagen a enviar con path: " + path<<endl;
    	archivo.open("ArchivoDeError.bmp", fstream::in | fstream::binary);
    	if(!archivo.good()) cout<<"Mal archivo error bmp"<<endl;
    }
    char * data=new char[2];
    archivo.read((char*)data,2);
    int tamano;
    archivo.read((char*)&tamano,4);
    archivo.close();
    archivo.open(path.c_str(), fstream::in | fstream::binary );
	unsigned int valorSend;
	streamsize extraidos;
	delete []data;
	data=new char[tamano];
	extraidos=archivo.readsome(data,tamano);
	valorSend = send(this->descriptorSocket, data, extraidos, 0);
	if (valorSend == -1) {cout<<"Mal enviado a servidor"<<endl; }
	archivo.close();
	delete []data;
	char* data2=new char[3];
	memset((void*)data2,'\0',3);
	data2[0]='e';
	data2[1]='o';
	data2[2]='f';
	int auxValorSend;
	auxValorSend=send(this->descriptorSocket,data2,3,0);
	delete []data2;
	return (int)auxValorSend;

}

char* Cliente::recibirRespuesta(){
        char* data=new char[MAXBYTESRECIBIDOS];
        memset((void*)data,'\0',MAXBYTESRECIBIDOS);
        bool seguir=true;
        ofstream* archivoResultado = new ofstream("recibido", ios::out);
        socklen_t leng=sizeof(char[MAXBYTESRECIBIDOS]);
        ssize_t valorRecive;
        while(seguir){
                valorRecive=recv(this->descriptorSocket,data,leng,0);
                if(valorRecive==0){
                cout<<"Se desconecto el servidor.."<<endl;
                cout<<"Se cerrará la aplicación"<<endl;
                sleep(2);
                exit(0);
                }
                if(valorRecive==-1){
                        cout<<"Mal recibido"<<endl;
                }else{
                        //corroboro que los ultimos tres formen eof
                        if((data[valorRecive-1]=='f')and(data[valorRecive-2]=='o')and(data[valorRecive-3]=='e')){
                                seguir=false;
                                ostringstream sstream;
                                sstream << data;
                                string lineaActual = sstream.str();
                                memset((void*)data,'\0',MAXBYTESRECIBIDOS);
                                //Para sacar el eof del archivo
                                string::iterator it=lineaActual.end();
                                it--;
                                it=lineaActual.erase(it);
                                it--;
                                it=lineaActual.erase(it);
                                it--;
                                it=lineaActual.erase(it);
                                *archivoResultado<<lineaActual;
                        }else{
                                ostringstream sstream;
                                sstream << data;
                                string lineaActual = sstream.str();
                                *archivoResultado<<lineaActual;
                                memset((void*)data,'\0',MAXBYTESRECIBIDOS);
                                delete []data;
                                data=new char[MAXBYTESRECIBIDOS];
                                memset((void*)data,'\0',MAXBYTESRECIBIDOS);
                        }
                }
        }
        delete []data;
        archivoResultado->close();
        delete archivoResultado;
        string recibido;
        string *recibidoAux=new string;
        ifstream* archivo=new ifstream("recibido");
        while(!archivo->eof()){
                std::getline(*archivo,*recibidoAux);
                recibido+=*recibidoAux;
                recibido+="\n";
        }
        char* dataAux=new char[recibido.size()];
        memset(dataAux,'\0',recibido.size());
        for(int i=0;i<recibido.size();i++) dataAux[i]=recibido[i];
        archivo->close();
        return dataAux;

}
string Cliente::recibirArchivo(string path){
        char* data=new char[MAXBYTESRECIBIDOS];
        memset((void*)data,'\0',MAXBYTESRECIBIDOS);
        bool seguir=true;
        ofstream* archivoResultado = new ofstream(path.c_str(), fstream::out | fstream::binary);
        socklen_t leng=sizeof(char[MAXBYTESRECIBIDOS]);
        ssize_t valorRecive;
        while(seguir){
				cout<<"recibe"<<endl;
                valorRecive=recv(this->descriptorSocket,data,leng,0);
                if(valorRecive==0){
                cout<<"Se desconecto el servidor.."<<endl;
                cout<<"Se cerrará la aplicación"<<endl;
                sleep(2);
                exit(0);
                }
                if(valorRecive==-1){
                        cout<<"Mal recibido"<<endl;
                }else{
                        //corroboro que los ultimos tres formen eof
                        if((data[valorRecive-1]=='f')and(data[valorRecive-2]=='o')and(data[valorRecive-3]=='e')){
                                seguir=false;
                                int bytes=valorRecive - 3;
                                char* final=new char[bytes];
                                for(int i=0;i<(valorRecive-3);i++) final[i]=data[i];
                                archivoResultado->write(final,bytes);
                                memset((void*)data,'\0',MAXBYTESRECIBIDOS);
                        }else{
                                archivoResultado->write(data,valorRecive);
                                memset((void*)data,'\0',MAXBYTESRECIBIDOS);
                                delete []data;
                                data=new char[MAXBYTESRECIBIDOS];
                                memset((void*)data,'\0',MAXBYTESRECIBIDOS);
                        }
                }
        }
        delete []data;
        archivoResultado->close();
        delete archivoResultado;
        return path;
}
Cliente::~Cliente() {
}
