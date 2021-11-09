/*
 * LEDMAT.h
 *
 *  Created on: 2021/11/09
 *      Author: st202109
 */

#ifndef SRC_LEDMAT_H_
#define SRC_LEDMAT_H_


class RGB {
	int data;
public:
	RGB(){
		data = 0x0000;
	}
	RGB(int data){
		this->data = data;
	}
	RGB(int r, int g, int b){
		this->setR(r)->setG(g)->setB(b);
	}
	int getR(){
		return (int)(data & 0x0f00 >> 8);
	}
	RGB* setR(int r){
		data &= 0xf0ff;
		data |= (r & 0x00f) << 8;
		return this;
	}
	int getG(){
		return (data & 0x00f0 >> 4);
	}
	RGB* setG(int g){
		data &= 0xf0ff;
		data |= (g & 0x00f) << 4;
		return this;
	}
	int getB(){
		return (data & 0x000f);
	}
	RGB* setB(int b){
		data &= 0xf0ff;
		data |= (b & 0x00f);
		return this;
	}
};

class LEDMAT {
public:
	LEDMAT();
	virtual ~LEDMAT();
};

#endif /* SRC_LEDMAT_H_ */
