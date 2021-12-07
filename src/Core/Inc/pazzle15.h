/*
 * pazzle15.h
 *
 *  Created on: 2021/11/01
 *      Author: st202109
 */

#ifndef INC_PAZZLE15_H_
#define INC_PAZZLE15_H_

#include <stdlib.h>

#include "mid_pazzle15.h"

#define BOARD_SIZE 4
#define BLANK_NUMBER 16


typedef struct TAG_POS{
	int row;
	int col;
} POS;

enum TMoveDirection{
	MOVE_NO_MOVE = -1,
	MOVE_LEFT,
	MOVE_DOWN,
	MOVE_UP,
	MOVE_RIGHT,
};


int pazzle_move(int);

POS get_blank_pos(void);
int move_blank(POS);

void board_init(void);
void board_shuffle(int, int);
int is_game_clear(void);
void waite_func(int);

#endif /* INC_PAZZLE15_H_ */
