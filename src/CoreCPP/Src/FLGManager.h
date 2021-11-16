/*
 * FLGManager.h
 *
 *  Created on: 2021/11/16
 *      Author: st202109
 */

#ifndef SRC_FLGMANAGER_H_
#define SRC_FLGMANAGER_H_

#include "pazzle_project.h"

class FLGManager {
private:
	unsigned int _flg;
public:
	FLGManager();
	virtual ~FLGManager();

	void set_flg(int);
	bool has_flg(int);
	bool any_flg();
};

#endif /* SRC_FLGMANAGER_H_ */
