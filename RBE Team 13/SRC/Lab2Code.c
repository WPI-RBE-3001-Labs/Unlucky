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
volatile unsigned long systemTime;
volatile unsigned long intTime;
volatile double globalVal = 9;
volatile long highLinkErr;
volatile long lowLinkErr;
unsigned int PIDcheck = FALSE;
volatile int highSetP = 90;
volatile int lowSetP = 90;

ISR(TIMER0_COMPA_vect) {
	global++;
	if(global >=globalVal){
		global = 0;
		systemTime++;
		intTime++;
		if(intTime >=10){
			PIDcheck = TRUE;
		}
	}
}

float ADCData(int channel)
{
		readADC2(channel);
		value2 = ADCL;
		higByte2 = ADCH;
		count2 = value2 + (higByte2 << 8);
		//mV2 = (count2 * ((5.0 * 10 * 10 * 10) / 1023.0));
		if(channel == 2)
		{
			angle2 = (0.2287 * count2) - 35.307;
			return angle2;
		}
		if(channel == 3)
		{
			angle2 = (0.2393 * count2) - 55.305;

			return angle2;
		}
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
			sig0 = sig0 - 40;
			sig1 = sig1 + 40;
		} else //Keep going up
		{
			sig0 = sig0 + 40;
			sig1 = sig1 - 40;
		}
	}
	if (up == 0) //going down
			{
		if (sig0 < 100) //approaching min of DAC
				{
			up = 1; //go up
			sig0 = sig0 + 40;
			sig1 = sig1 - 40;
		} else //Keep going down
		{
			sig0 = sig0 - 40;
			sig1 = sig1 + 40;
		}
	}
	setDAC(2, sig0);
	setDAC(3, sig1);
	//printf("DAC0 = %u, DAC1 = %u\n\r",sig0,sig1);
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

//PID
void updatePID(char link, int setPoint){
	switch(link){
	case 'H':
		if(FALSE){
			setDAC(2,0);
			setDAC(3,0);
		}
		else{
			long pidNum = calcPID('H', setPoint, ADCData(3));
			if(pidNum>=0){
				setDAC(2, pidNum);
				setDAC(3,0);
			}
			else{
				setDAC(2,0);
				setDAC(3,-pidNum);
			}
		}
		break;
	case 'L':
		if(FALSE){
			setDAC(0,0);
			setDAC(1,0);
		}
		else{
			long pidNum = calcPID('L', setPoint, ADCData(2));
			if(pidNum>=0){
				setDAC(0,0);
				setDAC(1,pidNum);
			}
			else{
				setDAC(0, -pidNum);
				setDAC(1,0);
			}
		}
		break;
	}
}
//Lab2 Initialization
void initLab2() {
	timerInit();
	initADC(2);
	initSPI();
}
//Lab 2 Code
void Lab2Code() {
	//printf("Hello");
	printf("AngleL: %0.1f, AngleH: %0.1f\r\n",ADCData(2),ADCData(3));
	if(PIDcheck){
		updatePID('H', highSetP);
		updatePID('L', lowSetP);
		PIDcheck = FALSE;
	}
	/*setDAC(0,3000);
	setDAC(0,0);
	setDAC(2,3000);
	setDAC(3,0);*/

}

