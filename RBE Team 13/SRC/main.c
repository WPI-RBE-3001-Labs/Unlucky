/* main.c
 *
 *  Created on: August 20, 2016
 *      Author: Joest
 */
#include "RBELib/RBELib.h"
#include <stdlib.h>
//character for receiving serial data

int main(void) {
	initRBELib(); //Setup printf() and setServo()
	debugUSARTInit(115200); //Initialize UART
	initADC(7);
	DDRB = 0x00; //enable PORTB for switches
	PORTB = 0x00;

	DDRD = 1 << PIN5; //set PORTD as an output

	sei();
	while (1)
	{
		Lab1Code();
	}
	return 0;
}

