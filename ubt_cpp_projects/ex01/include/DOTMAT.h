/*
 * DOTMAT.h
 *
 *  Created on: 2021/11/15
 *      Author: st202109
 */

#ifndef SRC_DOTMAT_H_
#define SRC_DOTMAT_H_

#include "pazzle_project.h"
#include "RGBDATALINE.h"

#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

class DOTMAT {
protected:
	int width, height;
	std::vector<RGB_DATALINE> dot_data;
public:
	DOTMAT(int, int, int, int[]);
	DOTMAT(int,int);
	DOTMAT(int);
	DOTMAT();
	virtual ~DOTMAT();
	void init(int, int);
	void reserve_push();
	RGB_DATALINE operator[](int);

	void clr();

	void cpMat(int, int, int, int, DOTMAT &);
	void cpMat(int, int, int, int, int, int[]);
};

#endif /* SRC_DOTMAT_H_ */
