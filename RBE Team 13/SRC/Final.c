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
int flag = 0;
volatile int tickA = 0;
#define DefaultH 30;
#define DefaultL 70;
ISR(TIMER0_COMPA_vect)
{
	tickA ++; //100 every second
	flag = 1;
}
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
float getIR(){
	readADC2(5);
	float count = ADCL + (ADCH << 8);
	return count;
}
//IR: ADC5
int objDetect() {
	int val = 350; //with box
	if (getIR() > val) { //something on conveyer
		return TRUE;
	} else {
		return FALSE;
	}
}
//Calculates current value
float calcCurrent(float val) {
	//The resistor across the sensor is .5Ohms and gain is 20 so no division needed (20*0.05 = 1)
	int gain = 20;
	float res = .05;
	float offset = 2.7077;	//count2 @rest gives 554 so offset = 2.7077
	float Resolution = 5 / 1023; //Vcc is 5
	float curr = ((float) ((val * Resolution) - offset)/gain/res);
	return curr;
}
//CurrentSense: Low Motor 0, High Motor 1
float getCurr(char motor) {
	switch(motor)
	{
	case 'H':
		readADC2(1);
		break;
	case 'L':
		readADC2(0);
		break;
	}
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
	//printf("State: %u \r\n", state);
	//printf("IR: %f \r\n", getIR());
	//Always run belt and update PID
	runBelt();
	//100Hz update
	if(flag == 1)
	{
		flag = 0;
		updatePID('H', highSetP);
		updatePID('L', lowSetP);
	}

	switch (state) {
	//Check IR for object
	case 0:
		openGrip();
		int chk = objDetect();
		if (chk) {state = 1;printf("Detected");}
		highSetP = 90;
		lowSetP = 90;
		break;
	//Move arm to wait position
	case 1:
		highSetP = DefaultH;
		lowSetP = DefaultL;
		if(reachPosition() == 1){state = 2;tickA = 0;printf("wait");} //has reached desired position
		break;
	//Move arm to grip position
	case 2:
		if(tickA > 460)//(Delay 4.6 seconds)
		{
			highSetP = 74;
			lowSetP = 0;
		if(reachPosition() == 1){state = 3;closeGrip();} //has reached desired position
		}
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
