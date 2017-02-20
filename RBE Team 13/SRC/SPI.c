/*
 * SPI.c
 *
 *  Created on: 14 Feb 2017
 *      Author: sangu
 */
#include "RBELib/RBELib.h"

void initSPI(void) {

	SPI_MASTER_SS = OUTPUT;
	SPI_MOSI = OUTPUT;
	SPI_SCK = OUTPUT;
	SPI_MISO = INPUT;

	//Bit 7: Disable interrupts
	//Bit 6: Enable the SPI lines
	//Bit 5: MSB transmitted first
	//Bit 4: Master mode
	//Bit 3: CLK low when idle
	//Bit 2: Sample on leading edge of signal
	//Bits 1 - 0: 128 CLK prescaler
	SPCR = 0x53;

	DAC_SS_ddr = 1;

	DAC_SS = 0;  //toggle DAC
	DAC_SS = 1;

	DDRDbits._P7 = OUTPUT;
	PORTDbits._P7 = 1;
	//Encoders SS lines = outputs
	ENCODER_SS_0_ddr = OUTPUT;
	ENCODER_SS_1_ddr = OUTPUT;
	//Deassert
	ENCODER_SS_0 = HIGH;
	ENCODER_SS_1 = HIGH;
}

unsigned char spiTransceive(BYTE data) {
	//start transmission
	SPDR = data;
	//wait for transmission to complete
	while (!(SPSR & 0x80))
		;
	unsigned char dataIn = SPDR;
	return SPDR;
}
