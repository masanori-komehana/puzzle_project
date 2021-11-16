/*
 * FLGManager.cpp
 *
 *  Created on: 2021/11/16
 *      Author: st202109
 */

#include "FLGManager.h"

FLGManager::FLGManager() {
	// TODO Auto-generated constructor stub
	_flg = 0;
}

FLGManager::~FLGManager() {
	// TODO Auto-generated destructor stub
}

void FLGManager::set_flg(int flg_bit){
	SET_FLG(_flg, flg_bit);
}

bool FLGManager::has_flg(int flg_bit){
	bool _f = false;
	if(HAS_FLG(_flg, flg_bit)){
		CLR_FLG(_flg, flg_bit);
		_f = true;
	}
	return _f;
}

bool FLGManager::any_flg(){
	return _flg != 0;
}
