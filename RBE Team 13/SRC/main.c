/* main.c
 *
 *  Created on: August 20, 2016
 *      Author: Joest
 */
#include "RBELib/RBELib.h"
#include <stdlib.h>
//character for receiving serial data
char inchar;
int lowADC = 0;
int highADC=1023;
float pot1ang = 0;
unsigned long potangint;
float mV=0;

volatile unsigned long sysTime = 0;
volatile unsigned long timerCounter;
volatile unsigned long intTime;
volatile double timerCountVal = 9;
int test = 10;
//Timer0 should be default 1843200Hz

ISR(TIMER0_OVF_vect){
	timerCounter++;
	if(timerCounter>=timerCountVal){
		timerCounter=0;
		sysTime++;
		intTime++;
		DDRC = 0xFF;
		test = PINC0;
	}
}


int main(void){
sei();
initRBELib(); //Setup printf() and setServo()
debugUSARTInit(115200); //Initialize UART
int value = 0;
int higByte = 0;
float count = 0;
float time = 0;
initADC(7);
while (1){
//ADC Reading
ADCSRA = ADCSRA | (1<<ADSC);
while((ADCSRA & (1<< ADSC)) >0){
	value = ADCL;
	higByte=ADCH;
	count = value + (higByte<<8);
	time = sysTime;
	float millis = (count*((5.0*10*10*10)/1023.0));
	float angle = (count*(270.0/1023.0));
	printf("Time: %f Angle: %f Count: %f mV: %f \n\r", time, angle, count, millis);
	timerCountVal = Q3(angle);
	printf(test);
}
}
return 0;
}


