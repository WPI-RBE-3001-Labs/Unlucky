/*
 * Final.c
 *
 *  Created on: 1 Mar 2017
 *      Author: sangu
 */
#include "RBELib/RBELib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

float highSetP = 90;
float lowSetP = 60;
int state = 0;
#define DefaultH 30;
#define DefaultL 70;
//Setup
void initFinal() {
	initADC(2);
	initSPI();
	setConst('H', 120, 0, 0); //look at this
	setConst('L', 120, 0, 0); //look at this
}
//LowLink: ADC2
float getAngL() {
	readADC2(2);
	float count = ADCL + (ADCH << 8);
	return (0.2211 * count) - 36.244; //our arm
	//return (0.2483 * count2) - 66.825; //team 8 arm
}
//HighLink: ADC3
float getAngH() {
	readADC2(3);
	float count = ADCL + (ADCH << 8);
	return (0.2311 * count) - 59.14; //our arm
	//return (0.2434 * count2) - 67.541; //team 8 arm
}
//IR: ADC5
int objDetect() {
	readADC2(5);
	int val = 350; //with box
	int IRout = ADCL + (ADCH << 8);
	if (IRout > val) { //something on conveyer
		return TRUE;
	} else {
		return FALSE;
	}
}
//Calculates current value
float calcCurrent(float val) {
	//The resistor across the sensor is .5Ohms
	float res = .05;
	float VCC = 5;
	float offset = 2.7077;	//count2 @rest gives 554 so offset = 2.7077
	float gain = 20;		//confirmed
	float Resolution = VCC / 1023;
	float curr = ((float) (((val * Resolution) - offset) / gain) / res);
	//printf("%f \n\r", curr);
	return curr;
}
//CurrentSense: ADC6
float getCurr() {
	readADC2(6);
	float count = ADCL + (ADCH << 8);
	float curr = calcCurrent(count);
	return curr;
}
//Servo 1
void closeGrip() {
	setServo(1, 180);
}
//Servo 1
void openGrip() {
	setServo(1, 0);
}
//Servo 0
void runBelt() {
	setServo(0, 0);	//back driven, 180 is forward but the wrong way for our set up
}
//Determine whether arm has reached set positions
int reachPosition()
{
	if (getAngL() >= (lowSetP - 2) && getAngL() <= (lowSetP + 2)) //got correct angles
		{
			if (getAngH() >= (highSetP - 2) && getAngH() <= (highSetP + 2)) //got correct angles
				{
					return 1;
				}
		}
	return 0;
}
//State space
void finalState() {
	printf("State: %u \r\n", state);
//		int current;
//		int lowWeight;
//		int highWeight;

	//Always run belt and update PID
	runBelt();
	updatePID('H', highSetP);
	updatePID('L', lowSetP);

	switch (state) {
	//Check IR for object
	case 0:
		openGrip();
		int chk = objDetect();
		if (chk) {state = 1;}
		highSetP = 90;
		lowSetP = 90;
		break;
	//Move arm to wait position
	case 1:
		highSetP = DefaultH;
		lowSetP = DefaultL;
		//Do we have to just hard time?
		if(reachPosition() == 1){state = 2; _delay_ms(4600);} //has reached desired position (Delay 4.6 seconds)
		break;
	//Move arm to grip position
	case 2:
		highSetP = 74;
		lowSetP = 0;
		if(reachPosition() == 1){state = 3;} //has reached desired position
		break;
	//Move arm to wait position
	case 3:
		highSetP = DefaultH;
		lowSetP = DefaultL;
		if(reachPosition() == 1){state = 4;} //has reached desired position
		break;
	case 4:
		//need to figure out current sampling at same time as arm moving
//				current = ADCData(6);
//				if(current <= lowWeight){
//					state = 5;
//				}
//				else if(current >= highWeight){
//					state = 6;
//				}
		break;
	case 5:
		//set w1 at distance 11.68 in
//				highSetP = 96.21;
//				lowSetP = 189.80;
//				updatePID('H', highSetP);
//				updatePID('L', lowSetP);
//				setServo(0,0);
//				state = 0;
		break;
	case 6:
		//set w2 at distance 7 in
//				highSetP = 192.71;
//				lowSetP = 149.56;
//				updatePID('H', highSetP);
//				updatePID('L', lowSetP);
//				setServo(0,0);
//				state = 0;
		break;

	}
}
