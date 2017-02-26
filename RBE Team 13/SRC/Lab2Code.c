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
volatile unsigned long systemTime =0;
volatile unsigned long intTime;

volatile double globalVal = 9;
unsigned int PIDcheck = FALSE;
float highSetP = 90;
float lowSetP = 60;
int state = 0;
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
float ADCData(int channel){
	readADC2(channel);
	count2 = ADCL + (ADCH << 8);
	if(channel == 2){
		return (0.2211 * count2) - 36.244;
	}
	else if(channel == 3){
		return (0.2311 * count2) - 59.14;
	}
	//current sense
	else if(channel == 6){
		float calcCurrent(int val) {
			//The resistor across the sensor is .5Ohms
			float res = .05;
			float VCC = 5;
			float offset = 2.7077;//count2 @rest gives 554 so offset = 2.7077
			float gain = 20;//confirmed
			float Resolution = VCC/1023;
			float curr = ((float) (((val * Resolution) - offset) / gain)/res);
			//printf("%f \n\r", curr);
			return curr;
		}
		float curr = calcCurrent(count2);
		return curr;
	}
	else{
		return count2;
	}
}

double pi = 3.1459;
//Angle Calculations//////////
double radtoDeg(double ang){
	return ang*(pi/180);
}
//TEST
//assuming links are both 6 inches long
float * getAngs(double px, double py){
	//can set these values. Trying to access them break it.
	float angles[2];
	angles[1] = radtoDeg(acos(((px*px + py*py) - 72)/72)); //theta 2
	float beta = radtoDeg(atan(py/px));
	float gam = radtoDeg(acos((px*px +py*py)/(12*sqrt(px*px+py*py)))); //theta 1
	angles[0] = beta + gam;
	return angles;
}
int objDetect(){
	readADC2(6);
	int val = 650;
	int IRout = ADCL + (ADCH << 8);
	if(IRout>val){//something on conveyer
		return TRUE;
	}else{
		return FALSE;
	}
}
//DAC Functions///////////////////////
void stopMotors(){
	setDAC(0,0);
	setDAC(1,0);
	setDAC(2,0);
	setDAC(3,0);
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
			//printf("pidNum: %0.1f\n\r", pidNum);
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

void setAngle() {
	if (PIND == 153) { //sw1
		highSetP = 0;
	} else if (PIND == 154) { //sw2
		highSetP = 30;
	} else if (PIND == 155) { //sw3
		highSetP = 45;
	} else if (PIND == 156){//sw4 is pressed
		highSetP = 60;
		}
	else{
		highSetP = 90;
	}
}

//Lab2 Initialization
void initLab2() {
	timerInit();
	initADC(2);
	initSPI();
	setConst('H', 120,0,0);
	setConst('L', 120,0,0);
}
//Lab 2 Code
void Lab2Code() {
	if(PIDcheck){
		int dx;
		setAngle();
		printf("AngL: %0.1f AngH: %f \r\n", ADCData(2), ADCData(3));
		updatePID('H', highSetP);
		updatePID('L', lowSetP);
		PIDcheck = FALSE;
		//test to see if it would build (can Delete)
		lowSetP = getAngs(6.5, 5.75)[0];
		highSetP = getAngs(6.5, 5.75)[1];
		//servo attempt
		setServo(5,90);
		//state space for final. We can work on this as we go
		/*
		switch(state)
		{
			case 0:
				//check IR sensor
				int chk = objDetect();
				if(chk){ //obj is on conveyor, switch to state 1
				dx = getDist();
				state = 1;
				}
				else{
				state = 0;
				}
				break;
			case 1:
				//move arm to wait pos
				int xpos = dx + 6.5;
				getAngs(xpos, 5.75);
				updatePID('H', highSetP);
				updatePID('L', lowSetP);
				//need to set waittime
				state = 2;
				break;
			case 2:
				int xpos = dx + 6.5;
				getAngs(xpos, 4.75);
				updatePID('H', highSetP);
				updatePID('L', lowSetP);
				setServo(0,140);
				state = 3;
				//move arm to grip pos
				break;
			case 3:
				//move arm to getweight pos
				updatePID('H', 90);
				updatePID('L', 90);
				state = 4;
				break;
			case 4:
				//need to figure out current sampling at same time as arm moving
				int current = ADCData(6);
				if(current <= lowweight){
					state = 5;
				}
				else if(current >= heighweight){
					state = 6;
				}
				break;
			case 5:
				//set w1 at distance 11.68 in
				highSetP = 96.21;
				lowSetP = 189.80;
				updatePID('H', highSetP);
				updatePID('L', lowSetP);
				setServo(0,0);
				state = 0;
				break;
			case 6:
				//set w2 at distance 7 in
				highSetP = 192.71;
				lowSetP = 149.56;
				updatePID('H', highSetP);
				updatePID('L', lowSetP);
				setServo(0,0);
				state = 0;
				break;

		}
		*/

	}

}

