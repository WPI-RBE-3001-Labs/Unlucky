/*
 * Lab1Code.c
 *
 *  Created on: 6 Feb 2017
 *      Author: sangu
 */
#include "RBELib/RBELib.h"
int Q3(float angle)
{
	int timerCountVal = 9;
	DDRB = 0x00;
		if(PINB1 == 1)
		{
			//1hz Prescaler of 256
			TCCR0B |= 1 << CS02;
			timerCountVal = 7200;
		}
		if(PINB3 == 1)
			{
				//20hz Prescaler of 8
			TCCR0B |= 1 << CS01;
			timerCountVal = 11520;
			}
		if(PINB5 == 1)
			{
				//100hz no prescaler
			TCCR0B |= 1 << CS00;
				timerCountVal = 18432;
			}

		return timerCountVal;
}

