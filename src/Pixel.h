#ifndef PIXEL_H_
#define PIXEL_H_

class Pixel {
private:
	char red;
	char green;
	char blue;
	unsigned long int posX,posY;
public:
	Pixel(char red,char green,char blue,unsigned long int posX, unsigned long int posY);
	char getRed();
	char getGreen();
	char getBlue();
	unsigned long int getPosX();
	unsigned long int getPosY();
	virtual ~Pixel();
};

#endif /* PIXEL_H_ */
