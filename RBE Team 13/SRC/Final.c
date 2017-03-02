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
float highSetP = 90;
float lowSetP = 60;
int state = 0;
int flag = 0;
volatile int tickA = 0;
float maxIR = 0;
ISR(TIMER0_COMPA_vect) {
	tickA++; //100 every second
	flag = 1; //PID Flag
}
//Setup
void initFinal() {
	initADC(2);
	initSPI();
	timerInit();
	setConst('H', 100, 0, 0); //look at this
	setConst('L', 100, 0, 0); //look at this
}
//LowLink: ADC2
float getAngL() {
	readADC2(2);
	float count = ADCL + (ADCH << 8);
	//return (0.2211 * count) - 36.244; //our arm
	return (0.2483 * count) - 66.825; //team 8 arm
}
//HighLink: ADC3
float getAngH() {
	readADC2(3);
	float count = ADCL + (ADCH << 8);
	//return (0.2311 * count) - 59.14; //our arm
	return (0.2434 * count) - 67.541; //team 8 arm
}
//PID
void updatePIDF(char link, int setPoint) {
	switch (link) {
	case 'H':
		if (FALSE) {
			setDAC(2, 0);
			setDAC(3, 0);
		} else {
			long pidNum = calcPID('H', setPoint, getAngH());
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
	if (getIR() > 350) { //something on conveyer
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
	//float curr = calcCurrent(count);
	return count;
}
//Get an average current value then return H for heavy or L for light
char weightCheck() {
	int currents[100];
	for (int i = 0; i < 100; i++) {
		currents[i] = getCurr('H');
	}
	long currentTot = 0;
	for (int i = 0; i < 100; i++) {
		currentTot += currents[i];
	}
	long currentAvg = currentTot / 100;
	printf("CA %lu\r\n", currentAvg);
	//This is not actually a tested value as of now
	if(currentAvg > 450){return 'H';}
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
//Determine whether arm has reached set positions
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
		//printf("H: %0.1f,L: %0.1f,IR: %0.1f \r\n",getAngH(),getAngL(),getIR());
		flag = 0;
	}
	switch (state) {

	//Check IR for object
	case 0:
		openGrip();
		//printf("IR: %f \r\n", getIR());
		int chk = objDetect();
		if (chk) {state = 1;printf("Detected \r\n");}
		highSetP = 70;
		lowSetP = 70;
		break;

	//Move arm to wait position
	case 1:
		highSetP = -15;
		lowSetP = 70;
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
		if (tickA > 380) //Delay 3.8 seconds
		{
			findPos();
			if (reachPosition() == 1) {
				state = 3;
				closeGrip();
				tickA = 0;
				getY(lowSetP, highSetP);
				getX(lowSetP, highSetP);
				//printf("Max %f\r\n", maxIR);
		}
		}
		break;

	//Move arm to wait position
	case 3:
		if (tickA > 75) //Delay 0.75 seconds
		{
			highSetP = 0;
			lowSetP = 90;
			//Determine Weight
			if (weightCheck() == 'H') {state = 5;printf("Heavy");}
			else if (weightCheck() == 'L') {state = 6;printf("Light");}
		}
		break;

	//case that does nothing for testing purposes
	case 4:
		break;

	//set w1 at distance 11.68 in (Lucy picked these angles)
	case 5:
				highSetP = 96.21;
				lowSetP = 189.80;
				if(reachPosition() == 1){openGrip();state = 0;printf("Restart");}
		break;

	//set w2 at distance 7 in (Lucy picked these angles)
	case 6:
				highSetP = 192.71;
				lowSetP = 149.56;
				if(reachPosition() == 1){openGrip();state = 0;printf("Restart");}
		break;

	}
}
