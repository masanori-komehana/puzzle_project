/*
 * LED_MATRIX3232.h
 *
 *  Created on: 2021/11/02
 *      Author: st202109
 */

#ifndef INC_LED_MATRIX3232_H_
#define INC_LED_MATRIX3232_H_

typedef short leddot_color;


typedef struct TAG_LEDMAT_LINE {
	int r;
	int g;
	int b;
} Ledmat_Line;

typedef struct TAG_LEDMAT_2LINE {
	Ledmat_Line line0;
	Ledmat_Line line1;
} Ledmat_2Lines;

int get_piece_data(int, int);

void out_LED_Matrix();


#endif /* INC_LED_MATRIX3232_H_ */
