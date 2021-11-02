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


// ���
enum TState{
	// �������
	STATE_INIT,
	// �p�Y���i�s��
	STATE_PAZZLE,
	//�@�N���A��ʕ\��
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

void set_flg();
int get_flg();

int is_timcnt();
void start_tim();
void stop_tim();
void toggle_tim();


int get_state();
void state_change(int);

int get_game_dat();

void disp_update();
void set_disp_data();

int get_disp_data(int, int);


#endif /* INC_MID_PAZZLE15_H_ */
