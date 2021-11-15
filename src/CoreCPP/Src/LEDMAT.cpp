/*
 * LEDMAT.cpp
 *
 *  Created on: 2021/11/09
 *      Author: st202109
 */

#include "LEDMAT.h"


RGB LEDMAT::DOT(int i, int j){
	return dot_data[i][j];
}

void LEDMAT::send_line(int pwm, int line_index, int oetime=23){
	RGB_DATALINE line0 = dot_data[line_index];
	RGB_DATALINE line1 = dot_data[line_index+16];
	int clk_cnt = 31;
	int line_bits = 0;

	for(int fclk_cnt=0;fclk_cnt<64;fclk_cnt++){
		// �N���b�N�����莞�̓���
		if ((clk==0)){
			// �N���b�N�t���O���]
			clk=1;
			// �N���b�N���J�E���g
			if(clk_cnt>=31){
				clk_cnt=0;
			}
			else
				clk_cnt++;
			//�N���b�N�o�� LoW
			GPIOB->ODR &= ~CLK_HI;       //SCK=0

			//�f�[�^����
			rgb_data = 0;
			rgb_data |= ((line0[clk_cnt].R() > pwm)?1:0) << 6;
			rgb_data |= ((line1[clk_cnt].R() > pwm)?1:0) << 7;
			rgb_data |= ((line0[clk_cnt].G() > pwm)?1:0) << 8;
			rgb_data |= ((line1[clk_cnt].G() > pwm)?1:0) << 9;
			rgb_data |= ((line0[clk_cnt].B() > pwm)?1:0) << 10;
			rgb_data |= ((line1[clk_cnt].B() > pwm)?1:0) << 11;
			//�f�[�^�o��
			GPIOA->ODR &= ~RGB_MASK | rgb_data ;
			GPIOA->ODR |= RGB_MASK & rgb_data ;
			//���b�`
			if(clk_cnt==31){
				GPIOB->ODR |=  STB_HI;      //SLATB =1
				line_bits = ((line_index & 0xc)<<2) |(line_index & 0x3);
				GPIOA->ODR &= ~LINE_MASK | line_bits ;
				GPIOA->ODR |= LINE_MASK & line_bits ;
			}
			if(clk_cnt==0){
				GPIOB->ODR &= ~ STB_HI;     //SLATB =0
			}
			//�o�̓C�l�[�u��
			if(clk_cnt==oetime){
				GPIOB->ODR |= OE_HI;      //OEB=1
			}
			if(clk_cnt==0){
				GPIOB->ODR &= ~OE_HI;     //OEB=0
			}
		}
		// �N���b�N�����オ�莞�̓���
		else{
			// �N���b�N�t���O���]
			clk=0;
			//�N���b�N�o�� Hi
			GPIOB->ODR |= CLK_HI;        //SCK=1
		}
	}
    GPIOB->ODR &= ~CLK_HI;       //SCK=0
}


