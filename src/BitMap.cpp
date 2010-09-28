
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
    	//guardo la informacion de en una lista de pixeles

    }
}

bool BitMap::seLevantoBienElArchivo(){
	return this->seLevantoCorrectamente;
}

bool BitMap::esUnaImagenCorrecta(){
	return ((this->seLevantoBienElArchivo())and(this->informacionImagen->profundidadColor==24)and(this->informacionImagen->bm[0]=='B')and(this->informacionImagen->bm[1]='M')and(this->informacionImagen->tamanoEstructura>0)and(this->informacionImagen->tipoCompresion==0));
}

BitMap::~BitMap() {
	delete this->informacionImagen;
	if(this->esUnaImagenCorrecta()){
		delete this->matrizDePixeles;
	}
}
