/*
 * Lab3Stuff.c
 *
 *  Created on: 2 Mar 2017
 *      Author: sangu
 */

#include "RBELib/RBELib.h"

#define M0Code 0x88
#define M1Code 0x90

int rvCount = 2047;

//get value from the accelerometer
signed int getAccel(int axis) {
	//might not need this section
	DDRDbits._P7 = OUTPUT;
	PORTDbits._P7 = 1;
	SPCR |= (1<<SPR1) + (1<<SPR0);

	//set
	signed int accelVal = 0x0000;
	signed int gVal = 0x0000;
	char temp0 = 0x00;
	int temp1 = 0x0000;
	int temp2 = 0x0000;

	PORTDbits._P7 = 0;
	temp0 = spiTransceive(0x06);
	temp1 = spiTransceive((axis<<6));
	temp2 = spiTransceive(0x00);
	PORTDbits._P7 = 1;
	temp1 &= (0x0F);
	accelVal |= ((temp1<<8) + temp2);
	//printf("%d\t%d\t\r\n", temp1, temp2);
	if (accelVal >= rvCount) {
		gVal = ((accelVal - rvCount)*.22);
	}
	else {
		gVal = -((rvCount - accelVal)*.22);
	}
	SPCR &= 0xFD;
	return gVal;
}

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
