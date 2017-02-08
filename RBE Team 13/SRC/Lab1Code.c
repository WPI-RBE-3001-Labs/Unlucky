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
int square = 0x00;
ISR(TIMER0_COMPA_vect) {
	timerCounter++;
	if(timerCounter == frequency/2){
		square = (!(square));
	}
	if(timerCounter>= 18000){
		time++;
		timerCounter = 0;
	}
}

void initCLK() {
	TCCR0A = (1<<WGM01)|(1<<COM0A0); //???
	TCCR0B = (1 << CS02) | (1 << CS00); //sets the timer0 prescaler to 1024
	//every time timerCounter == 1800?, 1843200/1024
	TIMSK0 = 0x2; //OCIEA enable

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
	printf("Time: %0.1f Angle: %0.1f Count: %0.1f mV: %0.1f \n\r", (float)time, angle, count, mV);
}

unsigned int dutyCycle = 50; //default duty cycle
void readSwitches() {

	if (PINB == 1) { //sw0
		frequency = 1;
	}
	else if (PINB == 2) { //sw1
		frequency = 20;
	}
	else if (PINB == 4) { //sw2
		frequency = 100;
	}
	else
	{
		frequency = 10; //default
	}
}
void setCLK() {
		PORTD = square;
}
int lastSeen = 0;
void Lab1Code() {
	initCLK();
	setCLK();
	readSwitches();


	if(time == lastSeen +1)
	{
		//Updates once a second
		//readADC();
		//PIND4 = 0
		printf("Frequency %d\n\r", frequency);
		float x = PINB;
		//printf("pinb1 %f \n\r", x);
		lastSeen = time;
		PINB = 0x00;
	}
	if(time%2 == 0)
	{
		//PIND4 = 1; //hook up LED to PORT D to test output
	}


}
