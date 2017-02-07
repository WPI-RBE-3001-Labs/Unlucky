/*
 * Lab1Code.c
 *
 *  Created on: 6 Feb 2017
 *      Author: sangu
 */
#include "RBELib/RBELib.h"

int time = 0;
volatile unsigned long timerCounter = 0;
ISR(TIMER0_COMPA_vect) {
	timerCounter++;
	if(timerCounter >= 18000)
	{
		time ++;
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
	else
	{
		frequency = 10; //default
	}
}
void setCLK() {
// Should set frequency and dutycycle when changed
	//divide clock general counter by frequncy?
}
int lastSeen = 0;
void Lab1Code() {
	initCLK();
	readSwitches();
	if(time == lastSeen +1)
	{
		//Updates once a second
		readADC();
		//PIND4 = 0
		lastSeen = time;
	}
	if(time%2 == 0)
	{
		//PIND4 = 1; //hook up LED to PORT D to test output
	}


}

