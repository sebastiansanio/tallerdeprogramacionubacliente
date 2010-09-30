
#include "BitMap.h"

BitMap::BitMap(string path) {

	this->informacionImagen=new informacionDelBMP;
    // referencia al archivo
    fstream  archivo;

    //intenta abrir el archivo en modo lectura y binario
    archivo.open(path.c_str(), fstream::in | fstream::binary );

    //verifico la apertura del mismo
    this->seLevantoCorrectamente=archivo.is_open();
    if (!this->seLevantoCorrectamente) {
        archivo.close();
    }else{
			//la primera parte del archivo son los datos del bitmap, los cargo en la estructura
			archivo.read((char *)&(this->informacionImagen->bm),2);
			archivo.read((char *)&(this->informacionImagen->tamano),sizeof(this->informacionImagen->tamano));
			archivo.read((char *)&(this->informacionImagen->reservado),sizeof(this->informacionImagen->reservado));
			archivo.read((char *)&(this->informacionImagen->offset),sizeof(this->informacionImagen->offset));
			archivo.read((char *)&(this->informacionImagen->tamanoMetadatos),sizeof(this->informacionImagen->tamanoMetadatos));
			archivo.read((char *)&(this->informacionImagen->anchoEnPixels),sizeof(this->informacionImagen->anchoEnPixels));
			this->bytesEnAncho=this->informacionImagen->anchoEnPixels*3;
			archivo.read((char *)&(this->informacionImagen->altoEnPixels),sizeof(this->informacionImagen->altoEnPixels));
			this->bytesEnAlto=this->informacionImagen->altoEnPixels*3;
			archivo.read((char *)&(this->informacionImagen->numeroPlanos),sizeof(this->informacionImagen->numeroPlanos));
			archivo.read((char *)&(this->informacionImagen->profundidadColor),sizeof(this->informacionImagen->profundidadColor));
			archivo.read((char *)&(this->informacionImagen->tipoCompresion),sizeof(this->informacionImagen->tipoCompresion));
			archivo.read((char *)&(this->informacionImagen->tamanoEstructura),sizeof(this->informacionImagen->tamanoEstructura));
			archivo.read((char *)&(this->informacionImagen->pixelPorMetroHorizontal),sizeof(this->informacionImagen->pixelPorMetroHorizontal));
			archivo.read((char *)&(this->informacionImagen->pixelPorMetroVertical),sizeof(this->informacionImagen->pixelPorMetroVertical));
			archivo.read((char *)&(this->informacionImagen->coloresUsados),sizeof(this->informacionImagen->coloresUsados));
			archivo.read((char *)&(this->informacionImagen->coloresImportantes),sizeof(this->informacionImagen->coloresImportantes));
    }
    if(this->esUnaImagenCorrecta()){
    	this->matrizDePixeles=new SDL_Color*[this->informacionImagen->altoEnPixels];
    	for(unsigned int i=0;i<this->informacionImagen->altoEnPixels;i++){
    		this->matrizDePixeles[i]=new SDL_Color[this->informacionImagen->anchoEnPixels];
    	}
    }
    //Corroboro los bytes de relleno
    int resto = this->informacionImagen->anchoEnPixels % 4;

    //Paso la informacion del archivo a la matriz
    for(unsigned int i=0;i<this->informacionImagen->altoEnPixels;i++){
    	for(unsigned int j=0; j<this->informacionImagen->anchoEnPixels;j++){
    		char color[3]=" ";
    		Uint8 rojo,verde,azul;
    		archivo.read((char*)&color,3);
    		rojo=(Uint8)color[2];
    		verde=(Uint8)color[1];
    		azul=(Uint8)color[0];
    		//Corroboro que el color no sea 0 255 0, que es lo que usamos para transparencia
    		if((rojo==0)and(verde==255)and(azul==0)){
    			this->matrizDePixeles[this->informacionImagen->altoEnPixels -1 - i][j].r=(Uint8)0;
    			this->matrizDePixeles[this->informacionImagen->altoEnPixels -1 - i][j].g=(Uint8)254;
       			this->matrizDePixeles[this->informacionImagen->altoEnPixels -1 - i][j].b=(Uint8)0;
    		}else{
    			this->matrizDePixeles[this->informacionImagen->altoEnPixels -1 - i][j].r=rojo;
    			this->matrizDePixeles[this->informacionImagen->altoEnPixels -1 - i][j].g=verde;
    			this->matrizDePixeles[this->informacionImagen->altoEnPixels -1 - i][j].b=azul;
    		}
    	}
    	//Quito los bytes de relleno
    	for(int i=0;i<resto;i++){
    		char bytes;
    		archivo.read(&bytes,1);
    	}
    }
}

bool BitMap::seLevantoBienElArchivo(){
	return this->seLevantoCorrectamente;
}

bool BitMap::esUnaImagenCorrecta(){
	return ((this->seLevantoBienElArchivo())and(this->informacionImagen->profundidadColor==24)and(this->informacionImagen->bm[0]=='B')and(this->informacionImagen->bm[1]='M')and(this->informacionImagen->tamanoEstructura>0)and(this->informacionImagen->tipoCompresion==0));
}

SDL_Color** BitMap::getMatrizDeImagen(){
	return this->matrizDePixeles;
}

bool BitMap::resizeTo(int alto, int ancho){
	SDL_Color** matrizAuxiliar = new SDL_Color*[alto];
	for(unsigned int i=0;i<alto;i++){
		matrizAuxiliar[i]=new SDL_Color[ancho];
	}
	float restoy=0;
	float restox=0;
	float coeficienteAlto=((float)this->informacionImagen->altoEnPixels)/(float)alto;
	float coeficienteAncho=((float)this->informacionImagen->anchoEnPixels)/(float)ancho;
	unsigned int y=0;
	unsigned int x=0;
	for(unsigned int i=0;i<alto;i++){
		restox=0;
		x=0;
		for(unsigned int j=0;j<ancho;j++){
			matrizAuxiliar[i][j]=this->matrizDePixeles[i][x];
			restox+=coeficienteAncho;
			if(restox>=1){
				x++;
				restox-=1;
			}
		}
	}
	this->informacionImagen->altoEnPixels=alto;
	this->informacionImagen->anchoEnPixels=ancho;
	delete this->matrizDePixeles;
	this->matrizDePixeles=matrizAuxiliar;
	return true;
}

unsigned int BitMap::getAlto(){
	return this->informacionImagen->altoEnPixels;
}

unsigned int BitMap::getAncho(){
	return this->informacionImagen->anchoEnPixels;
}
BitMap::~BitMap() {
	delete this->informacionImagen;
	if(this->esUnaImagenCorrecta()){
		delete this->matrizDePixeles;
	}
}
