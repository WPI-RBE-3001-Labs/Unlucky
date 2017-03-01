/*
 * timer.c
 *
 *  Created on: 1 Mar 2017
 *      Author: sangu
 */

#include "RBELib/RBELib.h"
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
