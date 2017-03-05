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
//Board is Team 1's
//Arm is Team 8's
//Programmer is ours
float highSetP = 90; //The angle to set the top link to
float lowSetP = 60; // The angle to set the bottom link to
int state = 0; // what part of the state space the program is in
int flag = 0; //determines if a sufficient time has passed for PID to be run again
volatile int tickA = 0; // timer counter
float maxIR = 0; //max IR value
//100Hz timer
ISR(TIMER0_COMPA_vect) {
	tickA++; //100 every second
	flag = 1; //PID Flag
}
//Setup
void initFinal() {
	initADC(2);
	initSPI();
	timerInit();
	setConst('H', 100, 0, 0);
	setConst('L', 100, 0, 0);
}
//LowLink: ADC2
float getAngL() {
	readADC2(2);
	float count = ADCL + (ADCH << 8);
	return (0.2483 * count) - 66.825; //conversion to angle
}
//HighLink: ADC3
float getAngH() {
	readADC2(3);
	float count = ADCL + (ADCH << 8);
	return (0.2434 * count) - 67.541; //conversion to angle
}
//Update PID
void updatePIDF(char link, int setPoint) {
	switch (link) {
	case 'H':
		if (FALSE) {
			setDAC(2, 0);
			setDAC(3, 0);
		} else {
			long pidNum = calcPID('H', setPoint, getAngH());
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
			long pidNum = calcPID('L', setPoint, getAngL());
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
//IR: ADC5
float getIR() {
	readADC2(5);
	float count = ADCL + (ADCH << 8);
	return count;
}
//Is there an object detected by IR
int objDetect() {
	if (getIR() > 370) { //something on conveyer
		return TRUE;
	} else {
		return FALSE;
	}
}
//Calculates current value
float calcCurrent(float val) {

	float offset = 2.70;
	float gain = 20.0;
	float resistorVal = .05;
	float VCC = 5.0;
	float ADCMAX = 1023;
	float curr = -200.0
			+ ((float) 100.0 * (val * VCC / ADCMAX) - offset) / gain
					/ resistorVal;
	return curr;
}
//CurrentSense: Low Motor 0, High Motor 1
float getCurr(char motor) {
	switch (motor) {
	case 'H':
		readADC2(1);
		break;
	case 'L':
		readADC2(0);
		break;
	}
	float count = ADCL + (ADCH << 8);
	return count;
}
//Get an average current value then return H for heavy or L for light
char weightCheck() {
	int currents[10];
	for (int i = 0; i < 10; i++) {
		currents[i] = getCurr('L');
	}
	long currentTot = 0;
	for (int i = 0; i < 10; i++) {
		currentTot += currents[i];
	}
	long currentAvg = currentTot / 10;
	printf("CA %lu\r\n", currentAvg);
	//This is not actually a tested value as of now
	if(currentAvg > 550){return 'H';}
	else {return 'L';}
}
//Servo 1
void closeGrip() {
	setServo(1, 220);
}
//Servo 1
void openGrip() {
	setServo(1, 0);
}
//Servo 0
void runBelt() {
	setServo(0, 0);	//back driven, 180 is forward but the wrong way for our set up
}
//Determine whether arm has reached set positions (angle tolerance of 3 degrees for both links)
int reachPosition() {
	if (getAngL() >= (lowSetP - 3) && getAngL() <= (lowSetP + 3)) //got correct angles
			{
		if (getAngH() >= (highSetP - 3) && getAngH() <= (highSetP + 3)) //got correct angles
				{
			return 1;
		}
	}
	return 0;
}
//determine where arm should grab based on weight location
void findPos() {
	//equations based off of testing math
	highSetP = -0.1863 * maxIR + 90.098;
	lowSetP = 0.0543 * maxIR + 15.776;
}
//State space
void finalState() {
	runBelt();	//Always run belt
	//100Hz update
	if (flag == 1) {
		updatePIDF('H', highSetP);
		updatePIDF('L', lowSetP);
		flag = 0;
	}
	switch (state) {

	//Check IR for object
	case 0:
		openGrip();
		maxIR = 0;
		int chk = objDetect();
		if (chk) {state = 1;printf("Detected \r\n");}
		highSetP = 70;
		lowSetP = 70;
		break;

	//Move arm to wait position
	case 1:
		highSetP = -5;
		lowSetP = 55;
		if (reachPosition() == 1) {
			state = 2;
			tickA = 0;
			printf("Wait\r\n");
		}
		break;

	//Move arm to grip position
	case 2:
		if (TRUE) {}; //Can't declare a variable after start of case for some reason
		float curIR = getIR();
		if (curIR > maxIR) {maxIR = curIR;} //finds distance with IR
		if (tickA > 375) //Delay 3.75  seconds
		{
			findPos();
			if (reachPosition() == 1) {
				state = 3;
				closeGrip();
				tickA = 0;
		}
		}
		break;

	//Move arm to wait position
	case 3:
		if (tickA > 75) //Delay 0.75 seconds
		{
			highSetP = 0;
			lowSetP = 90;
			if(reachPosition() == 1)
			{
			//Determine Weight
			if (weightCheck() == 'H') {state = 5;printf("Heavy\r\n");}
			else if (weightCheck() == 'L') {state = 6;printf("Light\r\n");}
			}
		}
		break;

	//case that does nothing for testing purposes
	case 4:
		break;

	//Drop off weight at H location
	case 5:
				highSetP = 95;
				lowSetP = 180;
				//Delays to drop weight correctly
				if(reachPosition() == 1){openGrip();_delay_ms(500);state = 0;printf("Restart\r\n");}
		break;

	//Drop off weight at L location
	case 6:
				lowSetP = 15;
				highSetP = 75;
				//Delays to drop weight correctly
				if(reachPosition() == 1){openGrip();_delay_ms(500); state = 0;printf("Restart\r\n");}
		break;
	}
}
