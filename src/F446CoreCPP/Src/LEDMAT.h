/*
 * LEDMAT.h
 *
 *  Created on: 2021/11/09
 *      Author: st202109
 */

#ifndef SRC_LEDMAT_H_
#define SRC_LEDMAT_H_

#ifdef __cplusplus
extern "C"{
#include "stm32f4xx_hal.h"
}
#endif
#include "DOTMAT.h"

#define A_HI 0x1
#define B_HI 0x2
#define C_HI 0x10
#define D_HI 0x20
#define LINE_MASK 0x33
#define R0_HI 0x40
#define R1_HI 0x80
#define G0_HI 0x100
#define G1_HI 0x200
#define B0_HI 0x400
#define B1_HI 0x800
#define RGB_MASK 0xFC0
#define CLK_HI 0x20
#define STB_HI 0x40
#define OE_HI 0x80

int getRGBdata(int, RGB, RGB);

class LEDMAT : public DOTMAT {
protected:
	int clk;
	int rgb_data;
public:
	// 継承元のコンストラクタ書かないと呼んでくれない
	LEDMAT(int w,int h,int color,int lines[])
	:DOTMAT(w, h, color, lines){}
	LEDMAT(int w,int h):DOTMAT(w, h){}
	LEDMAT(int size):DOTMAT(size) {}
	LEDMAT();
	RGB DOT(int,int);
	void send_line(int, int, int);


};

#endif /* SRC_LEDMAT_H_ */
