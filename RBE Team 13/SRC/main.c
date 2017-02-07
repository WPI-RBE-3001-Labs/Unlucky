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
initADC(7);

int value = 0;
int higByte = 0;
float count = 0;
float time = 0;
float frequency = 0;

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

}
		DDRB = 0x00;
		if(PINB1 == 1)
		{
			//1hz Prescaler of 256
			TCCR0B |= 1 << CS02;
			timerCountVal = 7200;
			frequency = 1;
		}
		if(PINB3 == 1)
			{
				//20hz Prescaler of 8
			TCCR0B |= 1 << CS01;
			timerCountVal = 11520;
			frequency = 20;
			}
		if(PINB5 == 1)
			{
				//100hz no prescaler
			TCCR0B |= 1 << CS00;
				timerCountVal = 18432;
				frequency = 100;
			}
printf(test);

}
return 0;
}


