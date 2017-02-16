/*
 * DAC.c
 *
 *  Created on: 14 Feb 2017
 *      Author: sangu
 */
#include "RBELib/RBELib.h"

void setDAC(int DACn, int SPIval) {

	unsigned char p1 = 0;
	unsigned char p2 = 0;
	unsigned char p3 = 0;
	unsigned long temp = 0;
	p1 = (0x30 | DACn); // write to and update DACn
	if (SPIval >= 4096){ // over max val
		SPIval = 4095;
	}
	//DAC_SS_ddr = OUTPUT; // portD4 set to output
	temp = SPIval;
	temp = temp >> 4; //bit shifted 4 bits right, 0 A B C -- > 0 0 [A B]
	p2 = temp;
	temp = SPIval & 0x000F;
	temp = temp << 4; //bit shifted 4 bits left, 0 A B C -- > A B [C 0]
	p3 = temp; // test
	DAC_SS = 0; //turn DAC on?

	spiTransceive(p1);
	spiTransceive(p2);
	spiTransceive(p3);

	DAC_SS = 1; //toggle to execute commands correctly
	DAC_SS = 0;
	DAC_SS = 1;

}

