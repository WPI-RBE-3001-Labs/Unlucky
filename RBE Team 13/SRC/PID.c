/*
 * PID.c
 *
 *  Created on: 20 Feb 2017
 *      Author: sangu
 */
#include "RBELib/RBELib.h"
//long pidErrorSum = 0;
long LastErrorL = 0;
long LastErrorH = 0;
long PrevLastErrorL = 0;
long PrevLastErrorH = 0;
long PrevL = 0;
long PrevH = 0;
float TdL = 0; //derivative time
float TdH = 0;
float TiL = 0; //reset time
float TiH = 0;

pidConst PID;
void setConst(char link, float Kp, float Ki, float Kd) {
	switch (link) {
	case 'H':
		PID.Kp_H = Kp;
		PID.Ki_H = Ki;
		PID.Kd_H = Kp;
		TdH = Kd / Kp;
		TiH = (Ki / Kp);
		break;
	case 'L':
		PID.Kp_L = Kp;
		PID.Ki_L = Ki;
		PID.Kd_L = Kd;
		TdL = Kd / Kp;
		TiL = (Ki / Kp);
		break;
	}
}

signed int calcPID(char link, int setPoint, int actPos) {
	long error = actPos - setPoint;
	long pidValue = 0;
	if (link == 'L') {
		pidValue = PrevL
				+ PID.Kp_L
						* ((1 + TiL + TdL) * error + (-1 - 2 * TdL) * LastErrorL
								+ TdL * PrevLastErrorL);
		PrevL = pidValue;
		PrevLastErrorL = LastErrorL;
		LastErrorL = error;
	} else {
		pidValue = PrevH
				+ PID.Kp_H
						* ((1 + TiH + TdH) * error + (-1 - 2 * TdH) * LastErrorH
								+ TdH * PrevLastErrorH);
		PrevH = pidValue;
		PrevLastErrorH = LastErrorH;
		LastErrorH = error;
	}
	if (pidValue > 4000)
		pidValue = 4000;
	if (pidValue < -4000)
		pidValue = -4000;
	return pidValue;
}

