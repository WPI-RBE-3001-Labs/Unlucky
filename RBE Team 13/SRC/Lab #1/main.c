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
ADMUX = (0<< REFS1)| //set up capacitor
		(1<< REFS0)|
		(0<< ADLAR)| //keep right adjust
		(0<< MUX4)| //To use analog 7
		(0<< MUX3)|
		(1<< MUX2)|
		(1<< MUX1)|
		(1<< MUX0);
ADCSRA = (1<< ADEN)| //ADC enable
		(0 << ADSC)| //no ADC start conversion
		(0 << ADATE)| //No ADC Auto Trigger
		(0 << ADIF)| //dont need to set the Flag
		(0 << ADIE)| //No ADC interupt enable
		(1 <<ADPS2)| //division factor 128 (smoothest read)
		(1 <<ADPS1)|
		(1 <<ADPS0);
ADCSRB = (0 << ACME)| //No comparitor
		(0 << ADTS2)| //free run mode?
		(0 << ADTS1)|
		(0 << ADTS0);
DIDR0 = (1 << ADC7D)| //disable 7 and no others
		(0 << ADC6D)|
		(0 << ADC5D)|
		(0 << ADC4D)|
		(0 << ADC3D)|
		(0 << ADC2D)|
		(0 << ADC1D)|
		(0 << ADC0D);

DDRB = 0xFF; //Set Port as output
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

