/*
 * Lab3Stuff.c
 *
 *  Created on: 2 Mar 2017
 *      Author: sangu
 */

#include "RBELib/RBELib.h"

#define M0Code 0x88
#define M1Code 0x90

int vref = 5;

//get value from the accelerometer 0 = x, 1 = y, 2 = z
int getAccel(int axis) {
	//set
	unsigned int accelVal = 0; //acceleration
	unsigned int gVal = 0;//g force
	//assert spare CS to enable communication between SPI and accelerometer
	PORTDbits._P7 = 0;
	//config bits
	spiTransceive(0x06);
	//get axis information
	accelVal = spiTransceive((axis<<6)); //Shifts  axis into the first two bits and sends zero bits
	accelVal = (accelVal & 0x0F) << 8; //Shifts the byte that was just read
	accelVal += spiTransceive(0x00); //sends zeros and recieves the rest of the data

	//deassert spare CS
	PORTDbits._P7 = 1;
	return accelVal;
	//convert acceleration value into Gforce
	if (accelVal >= vref) {
		gVal = ((accelVal - vref)*.22);
	}
	else {
		gVal = -((vref - accelVal)*.22);
	}
	return gVal;
}
//Initialise encoder
void encInit(int chan) {
	DDRCbits._P5 = OUTPUT;
	DDRCbits._P4 = OUTPUT;
	ENCODER_SS_0 = 1;
	ENCODER_SS_1 = 1;
	if (chan == 0) {
		ENCODER_SS_0 = 0;
		spiTransceive(M0Code);
		spiTransceive(0x03);
		ENCODER_SS_0 = 1;
		ENCODER_SS_0 = 0;
		spiTransceive(M1Code);
		spiTransceive(0x02);
		ENCODER_SS_0 = 1;
	}
	else if (chan == 1) {
		ENCODER_SS_1 = 0;
		spiTransceive(M0Code);
		spiTransceive(0x03);
		ENCODER_SS_1 = 1;
		ENCODER_SS_1 = 0;
		spiTransceive(M1Code);
		spiTransceive(0x02);
		ENCODER_SS_1 = 1;
	}

}
//reset encoder counter
void resetEncCount(int chan) {
	if (chan == 0) {
		ENCODER_SS_0 = 0;
		spiTransceive(0x20);
		ENCODER_SS_0 = 1;
	}
	else if (chan == 1) {
		ENCODER_SS_1 = 0;
		spiTransceive(0x20);
		ENCODER_SS_1 = 1;
	}
}
//determine encoder value
signed long encCount(int chan) {
	signed long value = 0x00000000;
	int data1 = 0;
	int data2 = 0;
	if (chan == 0) {
		ENCODER_SS_0 = 0;
		spiTransceive(0x60);
		data1 = spiTransceive(0x00);
		data2 = spiTransceive(0x00);
		ENCODER_SS_0 = 1;
	}
	else if (chan == 1) {
		ENCODER_SS_1 = 0;
		spiTransceive(0x60);
		data1 = spiTransceive(0x00);
		data2 = spiTransceive(0x00);
		ENCODER_SS_1 = 1;
	}
	value |= (data1<<8);
	value |= data2;
	return value;

}
