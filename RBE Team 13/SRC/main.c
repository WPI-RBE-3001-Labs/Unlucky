/* main.c
 *
 *  Created on: August 20, 2016
 *      Author: Joest
 */
#include "RBELib/RBELib.h"
#include <stdlib.h>

void initCLK() {
	TCCR0A = (1 << WGM01) | (1 << COM0A0); //???
	TCCR0B = (1 << CS02) | (1 << CS00); //sets the timer0 prescaler to 1024
	//every time timerCounter == 1800?, 1843200/1024
	TIMSK0 = 0x2; //OCIEA enable

}

int main(void) {
	initRBELib(); //Setup printf() and setServo()
	debugUSARTInit(115200); //Initialize UART
	initADC(7);
	DDRB = 0x00; //enable PORTB for switches
	PORTB = 0x00;
	DDRD = 0xFF; //set PORTD as an output
	initCLK();
	sei();
	while (1) {
		Lab1Code();
	}
	return 0;
}

