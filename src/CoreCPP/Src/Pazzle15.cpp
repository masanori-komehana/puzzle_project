/*
 * Pazzle15.cpp
 *
 *  Created on: 2021/11/02
 *      Author: st202109
 */

#include "Pazzle15.h"


Pazzle15::Pazzle15() {
	// TODO Auto-generated constructor stub
}

Pazzle15::Pazzle15(Pazzle15_mid &mid) {
	this->mid = &mid;
}

Pazzle15::~Pazzle15() {
	// TODO Auto-generated destructor stub
}

void Pazzle15::pazzle_move(int move_direction){
	int is_move = 0;
	POS blank_pos = get_blank_pos();
	POS mov_pos = blank_pos;
	switch (move_direction) {
	case MOVE_LEFT: {
		//is_timcnt = 1;
		is_move = 1;
		mov_pos.col++;
	}
		break;
	case MOVE_DOWN: {
		//is_timcnt = 0;
		is_move = 1;
		mov_pos.row--;
	}
		break;
	case MOVE_UP: {
		//if(!is_timcnt){
		//	clr_pzltim(game_dat.pzt);
		//}
		is_move = 1;
		mov_pos.row++;
	}
		break;
	case MOVE_RIGHT: {
		is_move = 1;
		mov_pos.col--;
	}
		break;
	default:
		break;
	}
	if(is_move){
		if (get_state() == STATE_INIT)
			state_change(STATE_PAZZLE);
		if (get_state() == STATE_PAZZLE){
			move_blank(mov_pos);
			if(is_game_clear()){
				state_change(STATE_CLEAR);
			}
		}
	}
}

POS Pazzle15::get_blank_pos(void)
{
    POS blank_pos = { -1, -1 };

    for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if(board[i][j] == BLANK_NUMBER){
				blank_pos.row = i;
				blank_pos.col = j;
			}
		}
	}

    return blank_pos;
}

void Pazzle15::move_blank(POS mov_pos)
{
    if (mov_pos.col < 0
		|| mov_pos.col >= BOARD_SIZE
		|| mov_pos.row < 0
		|| mov_pos.row >= BOARD_SIZE)
    {
        return;
    }

    POS blank_pos = get_blank_pos();

    // swapping
    int tempBlank = board[blank_pos.row][blank_pos.col];
    board[blank_pos.row][blank_pos.col] = board[mov_pos.row][mov_pos.col];
    board[mov_pos.row][mov_pos.col] = tempBlank;
}




void Pazzle15::board_init(void){
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			board[i][j] = i * BOARD_SIZE + j + 1;
		}
	}
}

void Pazzle15::board_shuffle(int times, int interval){
    for (int i = 0; i < times; ++i) {
		POS blank_pos = get_blank_pos();
		POS mov_pos = blank_pos;
		switch (rand()%4) {
		case 0:{ mov_pos.col++;} break;
		case 1:{ mov_pos.row--;} break;
		case 2:{ mov_pos.row++;} break;
		case 3:{ mov_pos.col--;} break;
		default: break;
		}
		move_blank(mov_pos);
		mid->disp_update();
		waite_func(interval);
	}
}

bool Pazzle15::Pazzle15::is_game_clear(void){
	bool flg_game_clear = true;
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			if(board[i][j] != i * BOARD_SIZE + j + 1){
				flg_game_clear = false;
			}
		}
	}
	return flg_game_clear;
}

void Pazzle15::pazzle_esc(void){
	switch(get_state()){
	case STATE_INIT:
		break;
	case -1:
	case STATE_PAZZLE:
	case STATE_CLEAR:
		state_change(STATE_INIT);
		break;
	}
}

void Pazzle15::state_change(int new_state){
	game_dat.flg |= FLG(CLS);
	if (game_dat.state == new_state){
		return;
	}
	switch(new_state){
	case STATE_INIT:
		game_dat.pzt.clr();
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

int Pazzle15::get_state(){
	return game_dat.state;
}
