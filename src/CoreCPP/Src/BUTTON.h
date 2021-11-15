/*
 * BUTTON.h
 *
 *  Created on: 2021/11/15
 *      Author: st202109
 */

#ifndef SRC_BUTTON_H_
#define SRC_BUTTON_H_

#ifdef __cplusplus
extern "C"{
#include "stm32f4xx_hal.h"
}
#endif


// 押しボタンスイッチ
enum TButtons{
	BTN_LEFT,
	BTN_DOWN,
	BTN_UP,
	BTN_RIGHT,
	BTN_A,
	BTN_B,
	BTN_ESC,
};

class BUTTON {
	GPIO_TypeDef * reg;
	int port_num;
	bool swc;
	bool swp1;
	bool swp2;
	bool now;
	bool last;
	bool up;
public:
	BUTTON();
	BUTTON(GPIO_TypeDef *, int);
	virtual ~BUTTON();
	bool is_push();
	bool is_posedge();
};

#endif /* SRC_BUTTON_H_ */
