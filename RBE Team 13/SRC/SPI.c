/*
 * SPI.c
 *
 *  Created on: 14 Feb 2017
 *      Author: sangu
 */
#include "RBELib/RBELib.h"

void initSPI() {
	//set MOSI and SCK output, all others input
	DDRB = (1 << SPI_MOSI) | (1 << SPI_SCK);
	//Enable SPI, Master, set clock rate fck/16
	SPCR = (1 << SPE) | (1 < MSTR) | (1 << SPR0);
}

unsigned char spiTransceive(BYTE data) {
	//start transmission
	SPDR = data;
	//wait for transmission to complete
	while (!(SPSR & (1 << SPIF)))
		;
}
