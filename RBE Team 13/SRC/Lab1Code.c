/*
 * Lab1Code.c
 *
 *  Created on: 6 Feb 2017
 *      Author: sangu
 */
#include "RBELib/RBELib.h"

volatile unsigned long frequency = 10;
int time = 0;
volatile unsigned long timerCounter = 0;
int i = 0;
volatile unsigned long globalCount = 0;
float dutyCycle = 50; //default duty cycle
float dutyCycle2 = 50;
int timeR = 0;


float angle = 0;
int value = 0;
int higByte = 0;
float count = 0;
float mV = 0;

void readADC() {
	ADCSRA = ADCSRA | (1 << ADSC);
	//while ((ADCSRA & (1 << ADSC)) > 0) {
	value = ADCL;
	higByte = ADCH;
	count = value + (higByte << 8);
	mV = (count * ((5.0 * 10 * 10 * 10) / 1023.0));
	angle = (count * (270.0 / 1023.0));
	dutyCycle = (count * (100.0 / 1023.0));
	dutyCycle2 = 100 - dutyCycle;
	//printf("Time: %d Angle: %0.1f Count: %0.1f mV: %0.1f \n\r", time, angle, count, mV);//this is the ADC values for Part 2
	printf("DC: %f PV: %0.1f F: %d S: %d  \n\r",
				dutyCycle, count, frequency, i); // This is print for part 3
	}


ISR(TIMER0_COMPA_vect) {
	timerCounter++;
	globalCount++;
	if (i == 1) {
		if (timerCounter >= (18000 / (frequency * (dutyCycle / 100)))) {
			PORTD = 0xFF;
			i = 0;
			timerCounter = 0;
		}
	} else if (i == 0) {
		if (timerCounter >= (1800 / (frequency * dutyCycle2 / 100))) {
			PORTD = 0x00;
			i = 1;
			timerCounter = 0;
		}
	}
	if (globalCount >= 18000) { //18000 = 1 sec
		time++;
		globalCount = 0;
	}
}

void initCLK() {
	TCCR0A = (1 << WGM01) | (1 << COM0A0); //???
	TCCR0B = (1 << CS02) | (1 << CS00); //sets the timer0 prescaler to 1024
	//every time timerCounter == 1800?, 1843200/1024
	TIMSK0 = 0x2; //OCIEA enable

}

void readSwitches() {
	//seems to be bug with SW2 registering too high initially
	//int x = PINB;
	//printf("%0.1d \n\r", x);
	if (PINB == 1) { //sw0
		frequency = 1;
	} else if (PINB == 2) { //sw1
		frequency = 20;
	} else if (PINB == 4) { //sw2
		frequency = 100;
	} else {
		frequency = 10; //default
	}
}

int lastSeen = 0;
void Lab1Code() {
	initCLK();
	readSwitches();
	readADC();
	PINB = 0x00; //resets switch so they always keep correct value

}
