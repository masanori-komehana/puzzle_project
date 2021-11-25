/*
 * DOTMAT.cpp
 *
 *  Created on: 2021/11/15
 *      Author: st202109
 */

#include "DOTMAT.h"

int sp_flg = 0;

DOTMAT::DOTMAT(int w,int h,int color,int lines[]) {
	init(w, h);
	for (int i = 0; i < h; ++i) {
		int flg = lines[i];
		RGB_DATALINE rgbline = dot_data[i];
		for (int j = 0; j < w; ++j) {
			// if(i == 0){
			// 	cout << "flg & FLG(w-j-1)" << '\n';
			// 	cout << hex << flg << " & " << FLG(w-j-1) << '\n';
			// }
			if(flg & FLG(w-j-1)) rgbline[j]->setColor(color);
			else rgbline[j]->setColor(0);
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
			dot_data[i][j]->setColor(0);
		}
	}
}

void DOTMAT::cpMat(int row, int col, int wid, int hei,
		DOTMAT& src_data){
	for (int i = 0; i < hei; ++i) {
		for (int j = 0; j < wid; ++j) {
			dot_data[row+i][col+j]->setColor(src_data[i][j]->getColor());
		}
	}
}

void DOTMAT::cpMat(int row, int col, int w,int h, int color,int lines[]) {
	for (int i = 0; i < h; ++i) {
		// int flg = lines[i];
		// for (int j = 0; j < w; ++j) {
		// 	if((flg) & (0x1 << (w-j-1))) dot_data[row+i][col+j].setColor(color);
		// 	else dot_data[row+i][col+j].setColor(0);
		// }

		int flg = lines[i];
		int flg2;
		RGB_DATALINE *rgbline = &dot_data[row+i];
		// if(i == 0) cout << "flg & FLG(w-j-1)" << endl;
		for (int j = 0; j < w; ++j) {
			flg2 = 0;
			if(i == 0){
				flg2 = (flg & FLG(w-j-1)) ? 1 : 0 ;
				// cout << hex << setw(8) << setfill('0') << flg << " & " << setw(8) << setfill('0') << FLG(w-j-1) << endl;
				// cout << resetiosflags(ios_base::floatfield);
				// cout << ((flg2)?"1":"0") << endl;
				sp_flg = 1;
			}
			// if(flg2) cout << hex << setw(3) << setfill('0') << rgbline[j]->getColor() << " -> ";
			if(flg & FLG(w-j-1)) rgbline->get(j)->setColor(color);
			else rgbline->get(j)->setColor(0);
			// if(flg2) cout << hex << setw(3) << setfill('0') << rgbline[j]->getColor() << endl;
		}
	}
}
