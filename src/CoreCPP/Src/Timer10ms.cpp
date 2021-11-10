/*
 * Timer10ms.cpp
 *
 *  Created on: 2021/11/09
 *      Author: st202109
 */

#include "Timer10ms.h"

Timer10ms::Timer10ms() {
	clr();
}

Timer10ms::Timer10ms(int m, int s, int ms) {
	min = m;
	sec = s;
	ms10 = ms;
}

void Timer10ms::clr() {
	min = 0;
	sec = 0;
	ms10 = 0;
}


Timer10ms::~Timer10ms() {
	// TODO Auto-generated destructor stub
}

