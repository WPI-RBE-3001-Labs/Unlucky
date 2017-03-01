/*
 * Kinematics.c
 *
 *  Created on: 1 Mar 2017
 *      Author: sangu
 */
#include "RBELib/RBELib.h"
#include <math.h>
//Placeholder values
double pi = 3.1459;
//for now length is 6?
double L1 = 6;
double L2 = 6;
//Converts radian value to degrees
double radToDeg(double ang) {
	return ang * (180 / pi);
}
//Converts degree value to radians
double degToRad(double ang) {
	return ang * (pi / 180);
}
//get x value
double getX(double theta1, double theta2){
	theta1 = degToRad(theta1);
	theta2 = theta2 * 3.14 / 180;
	double x1 = L1 * cos(theta1);
	double x2 = x1 + L2 * cos(theta1 + theta2);
	return x2;
}
