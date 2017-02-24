/*
 * Lab2Code.c
 *
 *  Created on: 12 Feb 2017
 *      Author: sangu
 */
#include "RBELib/RBELib.h"
//VARS
float count2 = 0;

volatile int global = 0;
volatile int secondaryCount = 0;
volatile unsigned long systemTime =0;
volatile unsigned long intTime;

volatile double globalVal = 9;
unsigned int PIDcheck = FALSE;
volatile int highSetP = 90;
volatile int lowSetP = 60;
int state = 0;
//TASK LIST
/* 1. try and get the final setup working.
		-mechanically in place; servos not working
	2. IR sensor location running
		-getting value, started detection function
		-function : objDetect works, needs tuning
	3. gripper and belt running
		- SERVOS NOT WORKING
		-Tried values in a range from 0 to 1000
		-It's very possible I set this up wrong
	4. hook up the current sense on the high
		- done
	5. see what that current equation gives
		-done, small error of abt .0014 A
	TODO Next steps:
	1. Servos working
	2. Figure out transforms and resultant joint angles
	3. Figure out conveyor speed so that we can grab at the correct time
	4. Kinematics stuff
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
int up = 1;
int sig0 = 0;
int sig1 = 4000;
void Triangle() {
	if (up == 1){ //going up
		if (sig0 > 4000){ //approaching max of DAC
			up = 0; //go down
			sig0 = sig0 - 40;
			sig1 = sig1 + 40;
		} else {//Keep going up
			sig0 = sig0 + 40;
			sig1 = sig1 - 40;
		}
	}
	if (up == 0){ //going down
		if (sig0 < 100){ //approaching min of DAC
			up = 1; //go up
			sig0 = sig0 + 40;
			sig1 = sig1 - 40;
		} else {//Keep going down
			sig0 = sig0 - 40;
			sig1 = sig1 + 40;
		}
	}
	setDAC(2, sig0);
	setDAC(3, sig1);
	//printf("DAC0 = %u, DAC1 = %u\n\r",sig0,sig1);
}

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

//To work on
void XY(){
	//figure out kinematics
	//zero should be low link 60-degree and high link 90-degree?
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
		setAngle();
		printf("Sensor: %0.1f Current: %f \r\n", ADCData(4), ADCData(6));
		updatePID('H', highSetP);
		updatePID('L', lowSetP);
		PIDcheck = FALSE;
		//state space for final. We can work on this as we go
		/*
		switch(state)
		{
			case 0:
				//check IR sensor
				int chk = objDetect();
				if(chk){ //obj is on conveyor, switch to state 1?
				state = 1;
				}
				else{
				state = 0;
				}
				break;
			case 1:
				//move arm down
				break;
			case 2:
				//gripper
				break;
			case 3:
				//move arm up
				break;
			case 4:
				//determine weight --> go to case 5 or 6
				break;
			case 5:
				//put weight on left
				//return to state 0
				break;
			case 6:
				//put weight on right
				//return to state 0
				break;

		}
		*/

	}

}

