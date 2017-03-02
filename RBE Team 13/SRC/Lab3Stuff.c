/*
 * Lab3Stuff.c
 *
 *  Created on: 2 Mar 2017
 *      Author: sangu
 */

#include "RBELib/RBELib.h"

#define CLR_CNTR 0x20
#define WRITE_MDR0 0x88
#define WRITE_MDR1 0x90

int rvCount = 2047;

/**
 * @brief Find the acceleration in the given axis (X, Y, Z).
 * @param  axis The axis that you want to get the measurement of.
 * @return gVal Value of  acceleration.
 *
 * @todo Create a function that is able to find the acceleration of a given axis.
 */
signed int getAccel(int axis) {
	DDRDbits._P7 = OUTPUT;
	PORTDbits._P7 = 1;
	SPCR |= (1<<SPR1) + (1<<SPR0);

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

/**
 * @brief Read an IR sensor and calculate the distance of the block.
 * @param  chan The port that the IR sensor is on.
 * @return value The distance the block is from the sensor.
 *
 * @todo Make a function that is able to get the ADC value of the IR sensor.
 */
int IRDist(int chan) {
	return 0;
}


void encInit(int chan) {
	DDRCbits._P5 = OUTPUT;
	DDRCbits._P4 = OUTPUT;
	ENCODER_SS_0 = 1;
	ENCODER_SS_1 = 1;
	if (chan == 0) {
		ENCODER_SS_0 = 0;
		spiTransceive(WRITE_MDR0);
		spiTransceive(0x03);
		ENCODER_SS_0 = 1;
		ENCODER_SS_0 = 0;
		spiTransceive(WRITE_MDR1);
		spiTransceive(0x02);
		ENCODER_SS_0 = 1;
	}
	else if (chan == 1) {
		ENCODER_SS_1 = 0;
		spiTransceive(WRITE_MDR0);
		spiTransceive(0x03);
		ENCODER_SS_1 = 1;
		ENCODER_SS_1 = 0;
		spiTransceive(WRITE_MDR1);
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
	//printf("%d\t%d\t\r\n", data1, data2);
	//resetEncCount(chan);
	return value;

}
