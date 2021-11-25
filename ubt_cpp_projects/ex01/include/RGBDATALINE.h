/*
 * RGBDATALINE.h
 *
 *  Created on: 2021/11/10
 *      Author: st202109
 */

#ifndef SRC_RGBDATALINE_H_
#define SRC_RGBDATALINE_H_

#include "pazzle_project.h"

#include "RGB.h"

#include <vector>


class RGB_DATALINE {

private:
	int length;
	std::vector<RGB> data_line;

public:
	//
	RGB_DATALINE(int);
	virtual ~RGB_DATALINE();
	// operator
	RGB& operator[](int i);
	//
	RGB& get(int i);
};

#endif /* SRC_RGBDATALINE_H_ */
