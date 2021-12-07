/*
 * pazzle15.c
 *
 *  Created on: 2021/11/01
 *      Author: st202109
 */


#include "pazzle15.h"


// ƒpƒYƒ‹‚Ì‹î
int board[4][4] = {
		{ 1, 2, 3, 4},
		{ 5, 6, 7, 8},
		{ 9,10,11,12},
		{13,14,15,16},
};



int pazzle_move(int move_direction){
	int is_move = 0;
	int moved = 0;
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
			moved = move_blank(mov_pos);
			if(is_game_clear()){
				state_change(STATE_CLEAR);
			}
		}
	}
	return moved;
}

POS get_blank_pos(void)
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

int move_blank(POS mov_pos)
{
    if (mov_pos.col < 0
		|| mov_pos.col >= BOARD_SIZE
		|| mov_pos.row < 0
		|| mov_pos.row >= BOARD_SIZE)
    {
        return 0;
    }

    POS blank_pos = get_blank_pos();

    // swapping
    int tempBlank = board[blank_pos.row][blank_pos.col];
    board[blank_pos.row][blank_pos.col] = board[mov_pos.row][mov_pos.col];
    board[mov_pos.row][mov_pos.col] = tempBlank;

    return 1;
}




void board_init(void){
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			board[i][j] = i * BOARD_SIZE + j + 1;
		}
	}
}

void board_shuffle(int times, int interval){
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
		disp_update();
		waite_func(interval);
	}
}

int is_game_clear(void){
	int flg_game_clear = 1;
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			if(board[i][j] != i * BOARD_SIZE + j + 1){
				flg_game_clear = 0;
			}
		}
	}
	return flg_game_clear;
}

