/*
 * SPI.c
 *
 *  Created on: 14 Feb 2017
 *      Author: sangu
 */
#include "RBELib/RBELib.h"

void initSPI(void) {

	SPI_MASTER_SS = OUTPUT;
	SPIDDRbits._MOSI = OUTPUT;
	SPIDDRbits._SCK = OUTPUT;
	SPIDDRbits._MISO = INPUT;

	//Bit 7: Disable interrupts
	//Bit 6: Enable the SPI lines
	//Bit 5: MSB transmitted first
	//Bit 4: Master mode
	//Bit 3: CLK low when idle
	//Bit 2: Sample on leading edge of signal
	//Bits 1 - 0: 128 CLK prescaler
	SPCR = 0x53;

	DDRC |= ((1 << DDC0) |	// spare SS
			(1 << DDC1) | // CoProcessor SS
			(1 << DDC2) | // spare SS
			(1 << DDC3) | // spare SS
			(1 << DDC4) | // encoder 1 SS
			(1 << DDC5) | // encoder 0 SS
			(1 << DDC6)); // spare SS

	//DAC
	DAC_SS_ddr = OUTPUT;
	DAC_SS = 0;  //toggle DAC
	DAC_SS = 1;
	//accel
	DDRDbits._P7 = OUTPUT;
	PORTDbits._P7 = 0;
	PORTDbits._P7 = 1;
	//Encoders SS lines = outputs
	ENCODER_SS_0_ddr = OUTPUT;
	ENCODER_SS_1_ddr = OUTPUT;
	//Deassert
	ENCODER_SS_0 = 1;
	ENCODER_SS_1 = 1;
}

unsigned char spiTransceive(unsigned char data) {
	//start transmission
	SPDR = data;
	//wait for transmission to complete
	while (!(SPSR & 0x80))
		;
	return SPDR;
}
