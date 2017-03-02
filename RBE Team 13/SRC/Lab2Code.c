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

void updatePID(char link, int setPoint) {
	switch (link) {
	case 'H':
		if (FALSE) {
			setDAC(2, 0);
			setDAC(3, 0);
		} else {
			long pidNum = calcPID('H', setPoint, ADCData(3));
			//printf("pidNum: %0.1f\n\r", pidNum);
			if (pidNum >= 0) {
				setDAC(2, pidNum);
				setDAC(3, 0);
			} else {
				setDAC(2, 0);
				setDAC(3, -pidNum);
			}
		}
		break;
	case 'L':
		if (FALSE) {
			setDAC(0, 0);
			setDAC(1, 0);
		} else {
			long pidNum = calcPID('L', setPoint, ADCData(2));
			if (pidNum >= 0) {
				setDAC(0, 0);
				setDAC(1, pidNum);
			} else {
				setDAC(0, -pidNum);
				setDAC(1, 0);
			}
		}
		break;
	}
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

