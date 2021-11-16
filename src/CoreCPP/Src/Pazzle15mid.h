/*
 * Pazzle15mid.h
 *
 *  Created on: 2021/11/09
 *      Author: st202109
 */

#ifndef SRC_PAZZLE15MID_H_
#define SRC_PAZZLE15MID_H_

#include "pazzle_project.h"

#include "Timer10ms.h"
#include "DOTMAT.h"
#include "LEDMAT.h"

class Pazzle15_mid {
private:
	LEDMAT *led_mat;
	DOTMAT disp_mat;
	Timer10ms tim;
	bool timer_active;
public:
	Pazzle15_mid();
	Pazzle15_mid(LEDMAT&);
	virtual ~Pazzle15_mid();

	void set_led_mat(LEDMAT& m){
		led_mat = &m;
	}


	bool is_timcnt();
	void start_tim();
	void stop_tim();
	void toggle_tim();


	int get_game_dat();

	void disp_update();
	void set_disp_data();

	DOTMAT get_piece_data(int);

	int get_disp_data(int, int);
};

#endif /* SRC_PAZZLE15MID_H_ */
