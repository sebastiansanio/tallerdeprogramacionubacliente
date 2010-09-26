/*
 * Pixel.h
 *
 *  Created on: 26/09/2010
 *      Author: damian
 */

#ifndef PIXEL_H_
#define PIXEL_H_

class Pixel {
private:
	char red;
	char green;
	char blue;
public:
	Pixel(char red,char green,char blue);
	char getRed();
	char getGreen();
	char getBlue();
	virtual ~Pixel();
};

#endif /* PIXEL_H_ */
