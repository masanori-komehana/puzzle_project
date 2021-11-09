/*
 * Pazzle15.h
 *
 *  Created on: 2021/11/02
 *      Author: st202109
 */

#ifndef SRC_PAZZLE15_H_
#define SRC_PAZZLE15_H_

#include <main.h>
#include <stdlib.h>

#include "Pazzle15mid.h"
#include "Timer10ms.h"

struct POS {
public:
	int row;
	int col;
};


// ゲーム管理データ
typedef struct TAG_GAME_DATA{
	int flg;
	int state;
	int posedge_buttons;
	Timer10ms pzt;
} GAME_DATA;

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

enum TMoveDirection{
	MOVE_NO_MOVE = -1,
	MOVE_LEFT,
	MOVE_DOWN,
	MOVE_UP,
	MOVE_RIGHT,
};

class Pazzle15 {
private:
	static const int BOARD_SIZE = 4;
	static const int BLANK_NUMBER = 16;
	int board[4][4];
	GAME_DATA game_dat;
	Pazzle15_mid * mid;
	int timer_state;
public:
	Pazzle15();
	Pazzle15(Pazzle15_mid &mid);
	virtual ~Pazzle15();

	void pazzle_move(int);

	POS get_blank_pos(void);
	void move_blank(POS);
	void pazzle_esc(void);

	void board_init(void);
	void board_shuffle(int, int);
	bool is_game_clear(void);

	int get_state();
	void state_change(int);
};

#endif /* SRC_PAZZLE15_H_ */
