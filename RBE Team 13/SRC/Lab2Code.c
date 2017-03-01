/*
 * Lab2Code.c
 *
 *  Created on: 12 Feb 2017
 *      Author: sangu
 */
#include "RBELib/RBELib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
//VARS
float count2 = 0;

volatile int global = 0;
volatile int secondaryCount = 0;
volatile unsigned long systemTime = 0;
volatile unsigned long intTime;

volatile double globalVal = 9;
unsigned int PIDcheck = FALSE;
float Theta2 = 90;
float Theta1 = 60;

//TASK LIST
/* 	Done today:
 1. getAngs function - NEEDS TO BE TESTED - Also program is Error1 ing, not sure
 why - please check
 2. Transforms were calculated
 3. cases 5 and 6 written, need to be tested when arm is fixed
 4. Moved triangle function into DAC.c
 5. Cases 1,2,3 written, need to be tested when arm is fixed
 6. Made solidworks sketch for joint positions - will be included in report

 TODO Next steps:
 1. FIX ARM
 2. Figure out conveyor speed so that we can grab at the correct time
 3. Servos working
 4. Map IR to inches
 5. Get weight detection working
 6. Get weights for testing

 */

//Timer Functions
//ISR(TIMER0_COMPA_vect) {
//	global++;
//	if (global >= globalVal) {
//		global = 0;
//		systemTime++;
//		intTime++;
//		if (intTime >= 10) {
//			PIDcheck = TRUE;
//		}
//	}
//}
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

//ADC Functions/////////
float ADCData(int channel) {
	readADC2(channel);
	count2 = ADCL + (ADCH << 8);
	//return count2;
	if (channel == 2) {
		return (0.2211 * count2) - 36.244; //our arm
		//return (0.2483 * count2) - 66.825; //team 8 arm
	} else if (channel == 3) {
		return (0.2311 * count2) - 59.14; //our arm
		//return (0.2434 * count2) - 67.541; //team 8 arm
	}
	//current sense
	else if (channel == 6) {
		float calcCurrent(int val) {
			//The resistor across the sensor is .5Ohms
			float res = .05;
			float VCC = 5;
			float offset = 2.7077;	//count2 @rest gives 554 so offset = 2.7077
			float gain = 20;		//confirmed
			float Resolution = VCC / 1023;
			float curr = ((float) (((val * Resolution) - offset) / gain) / res);
			printf("%f \n\r", curr);
			return curr;
		}
		float curr = calcCurrent(count2);
		return curr;
	} else {
		return count2;
	}
}

double pi = 3.1459;
//Angle Calculations//////////
double radtoDeg(double ang) {
	return ang * (pi / 180);
}
//TEST
//assuming links are both 6 inches long
float * getAngs(double px, double py) {
	//can set these values. Trying to access them break it.
	//this will always return basically 0 :(
	float angles[2];
	angles[1] = radtoDeg(acos(((px * px + py * py) - 72) / 72)); //theta 2
	float beta = radtoDeg(atan(py / px));
	float gam = radtoDeg(
			acos((px * px + py * py) / (12 * sqrt(px * px + py * py)))); //theta 1
	angles[0] = beta + gam;
	return angles;
}

void setAngle() {
	if (PIND == 153) { //sw1
		Theta2 = 0;
	} else if (PIND == 154) { //sw2
		Theta2 = 30;
	} else if (PIND == 155) { //sw3
		Theta2 = 45;
	} else if (PIND == 156) { //sw4 is pressed
		Theta2 = 60;
	} else {
		Theta2 = 90;
	}
}

//Lab2 Initialization
void initLab2() {
	timerInit();
	initADC(2);
	initSPI();
	setConst('H', 120, 0, 0);
	setConst('L', 120, 0, 0);
}
//Lab 2 Code
void Lab2Code() {
	if (PIDcheck) {
		setAngle();
		//printf("AngL: %0.1f AngH: %0.1f \r\n", ADCData(2), ADCData(3));
		printf("Current: %f \r\n", ADCData(6));
		updatePID('H', Theta2);
		updatePID('L', Theta1);
		PIDcheck = FALSE;
	}

}

