/*
 * Lab1Code.c
 *
 *  Created on: 6 Feb 2017
 *      Author: sangu
 */
#include "RBELib/RBELib.h"

volatile unsigned long timerCounter = 0;
unsigned long time = 0;
ISR(TIMER0_COMPA_vect) {
	timerCounter++;
	if(timerCounter >= 1800)
	{
		timerCounter = 0;
		time++;
	}
}

void initCLK() {
	TCCR0B = (1 << CS02) | (1 << CS00); //sets the timer0 prescaler to 1024
	//every time timerCounter == 1800

}

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
	printf("Time: %0.01f Angle: %0.001f Count: %0.1f mV: %0.1f \n\r", time, angle, count, mV);

	//}
}
float frequency = 10; //10Hz default
unsigned int dutyCycle = 50; //default duty cycle
void readSwitches() {
	if (PINB1 == 1) {
		frequency = 1;
	}
	if (PINB3 == 1) {
		frequency = 20;
	}
	if (PINB5 == 1) {
		frequency = 100;
	}
}
void setClock(int newFrequency) {

}
void Lab1Code() {
	readSwitches();
	readADC();
	initCLK();

}

