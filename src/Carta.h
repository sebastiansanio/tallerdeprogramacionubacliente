#ifndef CARTA_H_
#define CARTA_H_
using namespace std;
#include <string>
#include <string.h>

class Carta{
private:
	string path;
	string palo;
	string numero;
	int id;
public:
	Carta(string path, string palo, string numero, int id);
	string getPalo();
	string getNumero();
	string getPath();
	int getId();
	void setPath(string path);
	virtual ~Carta();

};



#endif /* CARTA_H_ */
