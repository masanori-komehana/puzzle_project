/*
 * DOTMAT.cpp
 *
 *  Created on: 2021/11/15
 *      Author: st202109
 */

#include "DOTMAT.h"

DOTMAT::DOTMAT(int w,int h,int color,int lines[]) {
	init(w, h);
	for (int i = 0; i < h; ++i) {
		int flg = lines[i];
		for (int j = w-1; j >= 0; --j) {
			if((flg >> (w-j-1)) & 0x1) dot_data[i][j] = color;
		}
	}
}

DOTMAT::DOTMAT(int w,int h) {
	// TODO Auto-generated constructor stub
	init(w, h);
}

DOTMAT::DOTMAT(int size) {
	init(size, size);
}

DOTMAT::DOTMAT() {
}

DOTMAT::~DOTMAT() {
	// TODO Auto-generated destructor stub
}

void DOTMAT::init(int w, int h){
	width = w;
	height= h;
	reserve_push();
}
void DOTMAT::reserve_push(){
	dot_data.reserve(height);
	for (int i = 0; i < height; ++i) {
		dot_data.push_back(RGB_DATALINE(width));
	}
}

RGB_DATALINE DOTMAT::operator[](int i){
	return dot_data[i];
}

void DOTMAT::clr(){
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			dot_data[i][j] = 0;
		}
	}
}

void DOTMAT::cpMat(int row, int col, int wid, int hei,
		DOTMAT& src_data){
	for (int i = 0; i < hei; ++i) {
		for (int j = 0; j < wid; ++j) {
			dot_data[row+i][col+j] = src_data[i][j];
		}
	}
}

void DOTMAT::cpMat(int row, int col, int w,int h, int color,int lines[]) {
	for (int i = 0; i < h; ++i) {
		int flg = lines[i];
		for (int j = 0; j < w; ++j) {
			if((flg) & (0x1 << (w-j-1))) dot_data[row+i][col+j].setColor(color);
			else dot_data[row+i][col+j].setColor(0);
		}
	}
}


