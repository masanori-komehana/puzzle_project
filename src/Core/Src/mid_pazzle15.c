/*
 * mid_pazzle15.c
 *
 *  Created on: 2021/11/01
 *      Author: st202109
 */
#include "mid_pazzle15.h"

extern int rgb_data1[16][3];
extern int rgb_data2[16][3];

// –¾‚é‚¢•û
extern int data_r0a[16];
extern int data_r1a[16];
extern int data_g0a[16];
extern int data_g1a[16];
extern int data_b0a[16];
extern int data_b1a[16];

// ˆÃ‚¢•û
extern int data_r0b[16];
extern int data_r1b[16];
extern int data_g0b[16];
extern int data_g1b[16];
extern int data_b0b[16];
extern int data_b1b[16];

extern GAME_DATA game_dat;

int timer_state = 0;

void disp_update(){
	cl_disp_data();
	cp_data(0, rgb_data1,
			data_r0a, data_r1a,
			data_g0a, data_g1a,
			data_b0a, data_b1a);
	cp_data(1, rgb_data2,
			data_r0b, data_r1b,
			data_g0b, data_g1b,
			data_b0b, data_b1b);
}

int is_timcnt(){return timer_state;}
void start_tim(){timer_state = 1;}
void stop_tim(){timer_state = 0;}
void toggle_tim(){timer_state ^= 1;}

void state_change(int new_state){
	game_dat.flg |= FLG(CLS);
	if (game_dat.state == new_state){
		return;
	}
	switch(new_state){
	case STATE_INIT:
		clr_pzltim(game_dat.pzt);
		board_init();
		board_shuffle(1000, 2);
		break;
	case STATE_PAZZLE:
		timer_state = 1;
		break;
	case STATE_CLEAR:
		timer_state = 0;
		break;
	}
	game_dat.state = new_state;
}

int get_state(){
	return game_dat.state;
}
