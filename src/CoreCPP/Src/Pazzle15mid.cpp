/*
 * Pazzle15mid.cpp
 *
 *  Created on: 2021/11/09
 *      Author: st202109
 */

#include "Pazzle15mid.h"

Pazzle15_mid::Pazzle15_mid() {
	// TODO Auto-generated constructor stub
	disp_mat = DOTMAT(32, 32);
}

Pazzle15_mid::~Pazzle15_mid() {
	// TODO Auto-generated destructor stub
}



int Pazzle15_mid::is_timcnt(){return 0;}
void Pazzle15_mid::start_tim(){;}
void Pazzle15_mid::stop_tim(){;}
void Pazzle15_mid::toggle_tim(){;}


int Pazzle15_mid::get_game_dat(){return 0;}

void Pazzle15_mid::disp_update(){;}
void Pazzle15_mid::set_disp_data(){;}

int Pazzle15_mid::get_disp_data(int, int){return 0;}

DOTMAT Pazzle15_mid::get_piece_data(int i){
	static int frame_data[8] = {0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff};
	static int piece_data[16][8]={
		{0xff, 0xe7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xff,},// 1
		{0xff, 0xc3, 0xfb, 0xc3, 0xdf, 0xdf, 0xc3, 0xff,},// 2
		{0xff, 0xc3, 0xfb, 0xc3, 0xfb, 0xfb, 0xc3, 0xff,},// 3
		{0xff, 0xdb, 0xdb, 0xc3, 0xfb, 0xfb, 0xfb, 0xff,},// 4
		{0xff, 0xc3, 0xdf, 0xc3, 0xfb, 0xfb, 0xc3, 0xff,},// 5
		{0xff, 0xc3, 0xdf, 0xc3, 0xdb, 0xdb, 0xc3, 0xff,},// 6
		{0xff, 0xc3, 0xdb, 0xdb, 0xfb, 0xfb, 0xfb, 0xff,},// 7
		{0xff, 0xc3, 0xdb, 0xc3, 0xdb, 0xdb, 0xc3, 0xff,},// 8
		{0xff, 0xc3, 0xdb, 0xdb, 0xc3, 0xfb, 0xc3, 0xff,},// 9
		{0xff, 0xa1, 0xad, 0xad, 0xad, 0xad, 0xa1, 0xff,},// 10
		{0xff, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xff,},// 11
		{0xff, 0xa1, 0xbd, 0xa1, 0xaf, 0xaf, 0xa1, 0xff,},// 12
		{0xff, 0xa1, 0xbd, 0xa1, 0xbd, 0xbd, 0xa1, 0xff,},// 13
		{0xff, 0xad, 0xad, 0xa1, 0xbd, 0xbd, 0xbd, 0xff,},// 14
		{0xff, 0xa1, 0xaf, 0xa1, 0xbd, 0xbd, 0xa1, 0xff,},// 15
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
	};

	static int rgb_data[16] = {
		0x300, 0x310, 0x320, 0x330,
		0x030, 0x031, 0x033, 0x013,
		0x003, 0x301, 0x303, 0x203,
		0x311, 0x131, 0x113, 0x000,
	};

	DOTMAT piece(8, 8, tcolor_WHITE, frame_data);

	if (i < 0 || i > 15){
		return DOTMAT(8, 8);
	}
	piece.cpMat(0, 0, 8, 8, rgb_data[i], piece_data[i]);
	return piece;
}
