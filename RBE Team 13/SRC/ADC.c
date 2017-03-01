#include "RBELib/RBELib.h"

/*
 * 50khz - 200khz
 * Free running - sample constantly
 * 13 cycles a conversion - 25 1st
 * P 246 - proper changing of channels
 *
 * ADC = (VIN * 1024) / Vref
 */
void initADC(int channel) {
	//Bits 7 - 6: Coupling capacitor at AREF
	//Bit 5: No left adjustment
	//Bits 4 - 0: Channel selection (0 - 7 for single ended)
	ADMUX = (0x40) | channel;

	ADCSRA = (1 << ADEN) | //ADC enable
			(0 << ADSC) | //no ADC start conversion
			(0 << ADATE) | //No ADC Auto Trigger
			(0 << ADIF) | //dont need to set the Flag
			(0 << ADIE) | //No ADC interupt enable
			(1 << ADPS2) | //division factor 128 (smoothest read)
			(1 << ADPS1) | (1 << ADPS0);
	ADCSRB = (0 << ACME) | //No comparitor
			(0 << ADTS2) | //free run mode?
			(0 << ADTS1) | (0 << ADTS0);
	DIDR0 = (1 << ADC7D)
			| //disable 7 and no others
			(0 << ADC6D) | (0 << ADC5D) | (0 << ADC4D) | (0 << ADC3D)
			| (0 << ADC2D) | (0 << ADC1D) | (0 << ADC0D);

	DDRB = 0xFF; //Set Port as output
}
unsigned short readADC2(int channel) {
	// select the corresponding channel 0~7
	// ANDing with '7' will always keep the value
	// of 'channel' between 0 and 7
	channel &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8) | channel;   // clears the bottom 3 bits before ORing
	ADCSRA = ADCSRA | (1 << ADSC);
	while (!(ADCSRA & (1 << ADIF)))
		;
	//Clear ADIF by writing one to it
	ADCSRA = ADCSRA | (1 << ADIF);
	return ADC;
}
