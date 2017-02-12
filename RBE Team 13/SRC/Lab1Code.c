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
int butP = 0;

float angle = 0;
int value = 0;
int higByte = 0;
float count = 0;
float mV = 0;

ISR(TIMER0_COMPA_vect) {
	timerCounter++;
	globalCount++;
	if (globalCount >= 18000) { //18000 = 1 sec
		time++;
		globalCount = 0;
		frequency = 10; //resets frequency
	}
}

void outputPWM()
{
	if (i == 1) {
		if (timerCounter >= (18000 / (frequency * (dutyCycle / 100)))) {
			PORTD = 0xFF;
			i = 0;
			timerCounter = 0;
		}
	} else if (i == 0) {
		if (timerCounter >= (18000 / (frequency * (dutyCycle2 / 100)))) {
			PORTD = 0x00;
			i = 1;
			timerCounter = 0;
		}
	}
}
void readADC() {
	ADCSRA = ADCSRA | (1 << ADSC);
	value = ADCL;
	higByte = ADCH;
	count = value + (higByte << 8);
	mV = (count * ((5.0 * 10 * 10 * 10) / 1023.0));
	angle = (count * (270.0 / 1023.0));
	float timeS = time + (globalCount / 18000.f);
	printf("Time: %f Angle: %0.1f Count: %0.1f mV: %0.1f \n\r", timeS, angle,
			count, mV); //this is the ADC values for Part 2
}
void readPWM() {
	ADCSRA = ADCSRA | (1 << ADSC);
	value = ADCL;
	higByte = ADCH;
	count = value + (higByte << 8);
	dutyCycle = (count * (100.0 / 1023.0));
	dutyCycle2 = 100 - dutyCycle;
	if (frequency == 225) //should stop printing after 1 second
			{
		printf("%d, %d \n\r", globalCount, i); //print for part 6
	} else {
		printf("DC: %f PV: %0.1f F: %d S: %d  \n\r", dutyCycle, count,
				frequency, i); // This is print for part 3 (state doesnt work correctly)
	}
	outputPWM();
}

void initCLK() {
	TCCR0A = (1 << WGM01) | (1 << COM0A0); //???
	TCCR0B = (1 << CS02) | (1 << CS00); //sets the timer0 prescaler to 1024
	//every time timerCounter == 1800?, 1843200/1024
	TIMSK0 = 0x2; //OCIEA enable

}

void readSwitches() {
	//seems to be bug with SW2 and SW4 deroing after SW4 is pressed
	if (PINB == 17) { //sw0
		frequency = 1;
	} else if (PINB == 18) { //sw1
		frequency = 20;
	} else if (PINB == 20) { //sw2
		frequency = 100;
	} else if (PINB == 0) //sw4 is pressed
			{
		if (frequency != 225 && butP == 0) //can only press once
				{
			globalCount = 0; //so its whole second.
			frequency = 225;
			butP = 1;
		}

	} else {
		frequency = 10; //default
	}
}

int lastSeen = 0;
void Lab1Code() {
	initCLK();
	readSwitches();
	//readADC();
	readPWM();
	PINB = 0x00; //resets switch so they always keep correct value

}
