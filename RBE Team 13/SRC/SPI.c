/*
 * SPI.c
 *
 *  Created on: 14 Feb 2017
 *      Author: sangu
 */
#include "RBELib/RBELib.h"

void initSPI() {
	SPI_MASTER_SS = OUTPUT;
	SPIDDRbits._MOSI = OUTPUT;
	SPIDDRbits._SCK = OUTPUT;
	SPIDDRbits._MISO = INPUT;
	SPCR = 0x53;
	DAC_SS_ddr = OUTPUT;
	DAC_SS = 0;
	DAC_SS = 1;
	DDRDbits._P7 = OUTPUT;
	PORTDbits._P7=1;
	ENCODER_SS_0_ddr = OUTPUT;
	ENCODER_SS_1_ddr = OUTPUT;
	ENCODER_SS_0 = 1;
	ENCODER_SS_1 = 1;
}

unsigned char spiTransceive(BYTE data) {
	//start transmission
	SPDR = data;
	//wait for transmission to complete
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}
