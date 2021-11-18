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
		// クロック立下り時の動作
		if ((clk==0)){
			// クロックフラグ反転
			clk=1;
			// クロック数カウント
			if(clk_cnt>=31){
				clk_cnt=0;
			}
			else
				clk_cnt++;
			//クロック出力 LoW
			GPIOB->ODR &= ~CLK_HI;       //SCK=0

			//データ統合
			rgb_data = 0;
			rgb_data |= ((line0[clk_cnt].R() > pwm)?1:0) << 6;
			rgb_data |= ((line1[clk_cnt].R() > pwm)?1:0) << 7;
			rgb_data |= ((line0[clk_cnt].G() > pwm)?1:0) << 8;
			rgb_data |= ((line1[clk_cnt].G() > pwm)?1:0) << 9;
			rgb_data |= ((line0[clk_cnt].B() > pwm)?1:0) << 10;
			rgb_data |= ((line1[clk_cnt].B() > pwm)?1:0) << 11;
			//データ出力
			GPIOA->ODR &= ~RGB_MASK | rgb_data ;
			GPIOA->ODR |= RGB_MASK & rgb_data ;
			//ラッチ
			if(clk_cnt==31){
				GPIOB->ODR |=  STB_HI;      //SLATB =1
				line_bits = ((line_index & 0xc)<<2) |(line_index & 0x3);
				GPIOA->ODR &= ~LINE_MASK | line_bits ;
				GPIOA->ODR |= LINE_MASK & line_bits ;
			}
			if(clk_cnt==0){
				GPIOB->ODR &= ~ STB_HI;     //SLATB =0
			}
			//出力イネーブル
			if(clk_cnt==oetime){
				GPIOB->ODR |= OE_HI;      //OEB=1
			}
			if(clk_cnt==0){
				GPIOB->ODR &= ~OE_HI;     //OEB=0
			}
		}
		// クロック立ち上がり時の動作
		else{
			// クロックフラグ反転
			clk=0;
			//クロック出力 Hi
			GPIOB->ODR |= CLK_HI;        //SCK=1
		}
	}
    GPIOB->ODR &= ~CLK_HI;       //SCK=0
}


int getRGBdata(int pwm, RGB rgb0, RGB rgb1){
	static int _rgb_data = 0;
	_rgb_data |= ((rgb0.R() > pwm)?1:0) << 6;
	_rgb_data |= ((rgb1.R() > pwm)?1:0) << 7;
	_rgb_data |= ((rgb0.G() > pwm)?1:0) << 8;
	_rgb_data |= ((rgb1.G() > pwm)?1:0) << 9;
	_rgb_data |= ((rgb0.B() > pwm)?1:0) << 10;
	_rgb_data |= ((rgb1.B() > pwm)?1:0) << 11;
	return _rgb_data;
}
