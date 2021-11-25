/*
 * RGBDATALINE.cpp
 *
 *  Created on: 2021/11/10
 *      Author: st202109
 */

#include "RGBDATALINE.h"

RGB_DATALINE::RGB_DATALINE(int size) {
	// TODO Auto-generated constructor stub
	length = size;
	data_line.reserve(size);
	for (int i = 0; i < length; ++i) {
		data_line.push_back(RGB(0));
	}
}

RGB_DATALINE::~RGB_DATALINE() {
	// TODO Auto-generated destructor stub
	for (int i = 0; i < length; ++i) {

	}
}


RGB* RGB_DATALINE::operator[](int i){
	RGB * rgb = &data_line[i];
	return rgb;
}
