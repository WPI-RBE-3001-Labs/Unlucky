/*
 * Lab2Code.c
 *
 *  Created on: 12 Feb 2017
 *      Author: sangu
 */
#include "RBELib/RBELib.h"

float angle2 = 0;
int value2 = 0;
int higByte2 = 0;
float count2 = 0;
float mV2 = 0;
volatile int global = 0;
volatile int secondaryCount = 0;
float DACcounts = 0;
volatile int toggle = 0;

ISR(TIMER0_COMPA_vect) {
	global++;
}

void readADC2() {
	ADCSRA = ADCSRA | (1 << ADSC);
	value2 = ADCL;
	higByte2 = ADCH;
	count2 = value2 + (higByte2 << 8);
	mV2 = (count2 * ((5.0 * 10 * 10 * 10) / 1023.0));
	angle2 = (0.2287 * count2) - 35.307; // angle = 0.2287*count2 - 35.307 is from nbest fit line to angle measurements
	printf("Angle: %0.1f Count: %0.1f mV: %0.1f \n\r", angle2, count2, mV2); //this is the ADC values for Part 2
}

int up = 1;
int sig0 = 0;
int sig1 = 4000;
void Triangle() {
	if (up == 1) //going up
			{
		if (sig0 > 4000) //approaching max of DAC
				{
			up = 0; //go down
			sig0 = sig0 - 20;
			sig1 = sig1 + 20;
		} else //Keep going up
		{
			sig0 = sig0 + 20;
			sig1 = sig1 - 20;
		}
	}
	if (up == 0) //going down
			{
		if (sig0 < 100) //approaching min of DAC
				{
			up = 1; //go up
			sig0 = sig0 + 20;
			sig1 = sig1 - 20;
		} else //Keep going down
		{
			sig0 = sig0 - 20;
			sig1 = sig1 + 20;
		}
	}
	setDAC(0, sig0);
	setDAC(1, sig1);
}

void timerInit() {
	//100hz timer
	TIMSK0 = (0 << OCIE0B) | (1 << OCIE0A) | //Enable compare A interrupt
			(0 << TOIE0);

	TCCR0A = (0 << COM0A1) | (0 << COM0A0) | (0 << COM0B1) | (0 << COM0B0)
			| (1 << WGM01) | (0 << WGM00);

	TCCR0B = (0 << FOC0A) | (0 << FOC0B) | (0 << WGM02) | (1 << CS02)
			| (0 << CS01) | //set clock divide to /1024
			(1 << CS00);
	OCR0A = 179; //should set to 100hz timer
	sei();
}
//Lab2 Initialization
void initLab2() {
	timerInit();
	initADC(2);
	initSPI();
}
//Lab 2 Code
void Lab2Code() {
	readADC2();
	Triangle();
}

