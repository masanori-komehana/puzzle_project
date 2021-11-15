/*
 * BUTTON.cpp
 *
 *  Created on: 2021/11/15
 *      Author: st202109
 */

#include "BUTTON.h"

BUTTON::BUTTON() {
	// TODO Auto-generated constructor stub

}
BUTTON::BUTTON(GPIO_TypeDef * gpiox, int p_n) {
	// TODO Auto-generated constructor stub
	reg = gpiox;
	port_num = p_n;
}

BUTTON::~BUTTON() {
	// TODO Auto-generated destructor stub
}

bool BUTTON::is_push(){
	static uint32_t buf, din;
	buf = reg->IDR;
	din = (((buf) >> (port_num)) & 0x01);
	return din != 0;
}

bool BUTTON::is_posedge(){
	swp2 = swp1;
	swp1 = swc;
	swc = is_push();
	if ((swp1 == swc) && (swp2 == swp1)) {
		now = swc;
	}

	if (now != last) {
		up = now && !last;
		last = now;
 	}
	return up;
}
