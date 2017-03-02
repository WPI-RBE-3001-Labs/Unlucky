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
//get y value
double getY(double theta1, double theta2){
	theta1 = degToRad(theta1);
	theta2 = theta2 * 3.14 / 180;
	double x1 = L1 * sin(theta1);
	double x2 = x1 + L2 * sin(theta1 + theta2);
	return x2;
}
//get theta2
double getTheta2(double x, double y)
{
	double x2 = x * x;
	double y2 = y * y;
	double L12 = L1 * L1;
	double L22 = L2 * L2;

	double theta2 = radToDeg(acos((x2 + y2 - L12 - L22)/(2 * L1 * L2)));
	return theta2;
}
//get theta1
double * getTheta1(double x, double y)
{
	double x2 = x * x;
	double y2 = y * y;
	double L12 = L1 * L1;
	double L22 = L2 * L2;

	double beta = radToDeg(atan2(y,x));
	double gam = radToDeg(acos((x2 + y2 + L12 + L22)/(2 * L1 * sqrt(x2 + y2))));
	double theta1[2];
	theta1[0] = beta + gam;
	theta1[1] = beta - gam;
	return theta1;
}

