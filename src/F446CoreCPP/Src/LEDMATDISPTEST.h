/*
 * LEDMATDISPTEST.h
 *
 *  Created on: 2021/11/16
 *      Author: st202109
 */


#ifndef SRC_LEDMATDISPTEST_H_
#define SRC_LEDMATDISPTEST_H_


#include "LEDMAT.h"

class LEDMAT_DISP_TEST {

public:
	LEDMAT mat;
public:
	LEDMAT_DISP_TEST() : mat(32){cp();}
	virtual ~LEDMAT_DISP_TEST();
	void cp();
};

#endif /* SRC_LEDMATDISPTEST_H_ */

