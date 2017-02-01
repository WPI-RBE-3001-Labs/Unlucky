/* main.c
 *
 *  Created on: August 20, 2016
 *      Author: Joest
 */

//I READ THIS
#include "RBELib/RBELib.h"
//For use of abs()
#include <stdlib.h>

//character for receiving serial data
char inchar;
int lowADC = 0;
int highADC=1023;

#include "RBELib/RBELib.h" //RBELib
int main(void){
initRBELib(); //Setup printf() and setServo()
debugUSARTInit(115200); //Initialize UART
int value = 0;
int higByte = 0;
int totalvalue = 0;

while (1){

	//Blink Light
PORTB = 0xFF; //Turn port on
_delay_ms(500); //Delay .5 sec
PORTB = 0x00; //Turn port off
_delay_ms(500); //Delay .5 sec
printf("Hello\n\r"); //Print “Hello”

	//ADC Reading
ADCSRA = ADCSRA | (1<<ADSC);
while((ADCSRA & (1<< ADSC)) >0)
{
	value = ADCL;
	higByte=ADCH;
	totalvalue = value + (higByte<<8);
	printf("new ADC value high %d low %d total %d \n\r", higByte, value, totalvalue);
}
}
return 0;
}

