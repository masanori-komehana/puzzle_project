/*
 * mid_pazzle15.h
 *
 *  Created on: 2021/11/01
 *      Author: st202109
 */

#ifndef INC_MID_PAZZLE15_H_
#define INC_MID_PAZZLE15_H_

#include "main.h"
#include "pazzle15.h"


// 状態
enum TState{
	// 初期状態
	STATE_INIT,
	// パズル進行中
	STATE_PAZZLE,
	//　クリア画面表示
	STATE_CLEAR,
};

enum TPazzleDifficulty{
	_EASY,
	_NORMAL,
};


enum TPazzleCmd{
	PCMD_NOP,
	PCMD_MOVE_LEFT,
	PCMD_MOVE_DOWN,
	PCMD_MOVE_UP,
	PCMD_MOVE_RIGHT,
	PCMD_ESC,
	PCMD_SELECT_DIFFICULTY,
	PCMD_TIM_START,
	PCMD_TIM_STOP,
	PCMD_TIM_CLR,
};

void cp_data(int, int[16][3], int[16], int[16], int[16], int[16], int[16], int[16]);
void waite_func(int);

void set_flg(void);
int get_flg(void);

int is_timcnt(void);
void start_tim(void);
void stop_tim(void);
void toggle_tim(void);


int get_state(void);
void state_change(int);

int get_game_dat(void);

void disp_update(void);
void set_disp_data(void);
void pazzle_esc(void);

int get_disp_data(int, int);


#endif /* INC_MID_PAZZLE15_H_ */
