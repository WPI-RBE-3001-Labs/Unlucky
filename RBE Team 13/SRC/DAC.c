/*
 * DAC.c
 *
 *  Created on: 14 Feb 2017
 *      Author: sangu
 */
#include "RBELib/RBELib.h"

unsigned char wordP1;
unsigned char wordP2;
unsigned char wordP3;

void setDAC(int DACn, int SPIval) {
	initSPI();
	if (SPIval > 4095) // over max val
			{
		SPIval = 4095;
	}
	DAC_SS_ddr = OUTPUT; // portD4 set to output
	wordP1 = (0x30 | DACn); // write to and update DACn
	int temp = SPIval;
	temp = temp >> 4; //bit shifted 4 bits right, 0 A B C -- > 0 0 [A B]
	wordP2 = temp;
	temp = SPIval;
	temp = temp << 4; //bit shifted 4 bits left, 0 A B C -- > A B [C 0]
	wordP3 = SPIval; // test
	DAC_SS = 0; //turn DAC on?

	spiTransceive(wordP1);
	spiTransceive(wordP2);
	spiTransceive(wordP3);

	DAC_SS = 1; //toggle to execute commands correctly
	DAC_SS = 0;
	DAC_SS = 1;

}

