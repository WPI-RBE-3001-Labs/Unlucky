/* main.c
 *
 *  Created on: August 20, 2016
 *      Author: sangu
 */
#include "RBELib/RBELib.h"
#include <stdlib.h>
int main(void) {
	initRBELib(); //Setup printf() and setServo()
	debugUSARTInit(115200); //Initialize UART
	//initLab1();
	//initLab2();
	initFinal();
	while (1) {
		//Lab1Code();
		//Lab2Code();
		finalState();
	}
	return 0;
}

