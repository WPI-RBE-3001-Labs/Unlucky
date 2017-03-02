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
//Link length in mm
float L1 = 150.0;
float L2 = 155.0;
//Converts radian value to degrees
double radToDeg(double ang) {
	return ang * (180 / pi);
}
//Converts degree value to radians
double degToRad(double ang) {
	return ang * (pi / 180);
}
//get x value
float getX(float theta1, float theta2) {
	theta1 = degToRad(theta1);
	theta2 = degToRad(theta2);
	float x1 = L1 * cos(theta1);
	float x2 = x1 + L2 * cos(theta1 + theta2);
	printf("X: %0.1f mm\r\n",x2);
	return x2;
}
//get y value
float getY(float theta1, float theta2) {
	theta1 = degToRad(theta1);
	theta2 = degToRad(theta2);
	float y1 = L1 * sin(theta1);
	float y2 = y1 + L2 * sin(theta1 + theta2);
	printf("Y: %0.1f mm\r\n",y2);
	return y2;
}
//get theta2
double getTheta2(double x, double y) {
	double x2 = x * x;
	double y2 = y * y;
	double L12 = L1 * L1;
	double L22 = L2 * L2;

	double theta2 = radToDeg(acos((x2 + y2 - L12 - L22) / (2 * L1 * L2)));
	return theta2;
}
//get theta1
double getTheta1(double x, double y) {
	double x2 = x * x;
	double y2 = y * y;
	double L12 = L1 * L1;
	double L22 = L2 * L2;

	double beta = radToDeg(atan2(y, x));
	double gam = radToDeg(
			acos((x2 + y2 + L12 - L22) / (2 * L1 * sqrt(x2 + y2))));
	double theta1 = beta + gam; //just assume + gam
	return theta1;
}

