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



class LEDMAT {
public:
	LEDMAT();
	virtual ~LEDMAT();
};

#endif /* SRC_LEDMAT_H_ */
