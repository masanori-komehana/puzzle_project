/*
 * Pazzle15mid.h
 *
 *  Created on: 2021/11/09
 *      Author: st202109
 */

#ifndef SRC_PAZZLE15MID_H_
#define SRC_PAZZLE15MID_H_

class Pazzle15_mid {
public:
	Pazzle15_mid();
	virtual ~Pazzle15_mid();


	int is_timcnt();
	void start_tim();
	void stop_tim();
	void toggle_tim();


	int get_game_dat();

	void disp_update();
	void set_disp_data();

	int get_disp_data(int, int);
};

#endif /* SRC_PAZZLE15MID_H_ */
