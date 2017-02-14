/*
 * Lab2Code.c
 *
 *  Created on: 12 Feb 2017
 *      Author: sangu
 */
#include "RBELib/RBELib.h"

#define EN0_LOW PORTC &= ~(1<<PORTC5);
#define EN0_HIGH PORTC &= (1<<PORTC5);
#define EN1_LOW PORTC &= ~(1<<PORTC4);
#define EN1_HIGH PORTC &= (1<<PORTC4);

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

// our timer is 100Hz not 1Hz for this lab. Youll have to change this to match that.
void setTriangle() {
	if (global >= 18000) { //18000 = 1 sec
		if (secondaryCount % 2 == 1) {
			toggle = 1;
		} else {
			toggle = 0;
		}
		secondaryCount++;
	}
}
void DACwrite() {
	setTriangle();
	float maxDAC = 4095; //maxvolts set to 5 volts, change if necessary
	DACcounts = 18000 - global;
	int sig1, sig2;
	float mapfactor = (DACcounts * maxDAC) / 18000;
	if (toggle == 1) { //rising for one, decreasing for the other
		sig1 = (DACcounts * mapfactor); //fall
		sig2 = global * mapfactor; //rise
	} else {
		sig1 = global * mapfactor; //rise
		sig2 = (DACcounts * mapfactor); //fall
	}
	setDAC(1, sig1);
	setDAC(2, sig2);
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
void initLab2() {
	timerInit();
	initADC(2);
	EN0_HIGH
	; // 0 degrees points towards the long base
	EN1_HIGH
	;
}
void Lab2Code() {
	readADC2();
	DACwrite();
}

