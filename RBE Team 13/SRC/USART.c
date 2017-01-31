/*
 * USART.c
 *
 *  Created on: Jan 26, 2015
 *      Author: joest
 */


#include "RBELib/RBELib.h"
//#include <avr/io.h>
//add comment
void debugUSARTInit(unsigned long baudrate){
	/*Set baud rate */
	UBRR1L = (CLK / (16 * baudrate)) -1;
	UBRR1H = ((CLK / (16 * baudrate)) -1 )>>8;
	//Control and status register B
	  //Bits 7 - 5: Diables interrupts
	  //Bit 4: enable Rx
	  //Bit 3: enable Tx
	  //Bit 2: Part of setting char size to 8 as per table 16-7 on 192
	  //Bits 1 - 0: Not used with 8 bit mode
	  UCSR1B = 0x18;

	  //Control and status register C
	  //Bits 7 - 6: Async mode
	  //Bits 5 - 4: No parity
	  //Bit 3: 1 stop bit
	  //Bits 2 - 1: 8 data bits
	  //Bit 0: Needs to be set to 0 when in async
	  UCSR1C = 0x06;

}

void putCharDebug(char byteToSend){
	  while(!(UCSR1A & 0x20));
	  UDR1 = byteToSend;
}

unsigned char getCharDebug(void) {
   return 'a';
}
