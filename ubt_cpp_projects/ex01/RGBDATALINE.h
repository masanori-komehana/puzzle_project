/*
 * RGBDATALINE.h
 *
 *  Created on: 2021/11/10
 *      Author: st202109
 */

#ifndef SRC_RGBDATALINE_H_
#define SRC_RGBDATALINE_H_

#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;
// TODO RGB.data を HEXで表示する処理

enum tColorVal{
	tcolor_RED    = 0x0f00,
	tcolor_GREEN  = 0x00f0,
	tcolor_BLUE   = 0x000f,
	tcolor_CYAN   = 0x00ff,
	tcolor_MAGENTA= 0x0f0f,
	tcolor_YELLOW = 0x0ff0,
	tcolor_WHITE  = 0x0fff,
};

// C#のプロパティ的な
struct RGB_prop{
	// 参照メンバーわからんのでポインタにした
	int* data;
	int color;

	int get(){return (*data >> color) & 0xf;}
	void set(int v){
		*data &= ~(0xf << color);
		*data |= (v & 0x00f) << color;
	}

	// 参考:　https://marycore.jp/prog/cpp/simple-property/
	// にあった使い方？
	operator int(){
		return get();
	}

	// キャストめんどいから参照時は括弧つける
	int operator()(){
		return get();
	}
	void operator()(int v){
		set(v);
	}

	void operator=(int v){
		set(v);
	}
};


class RGB {
	int data;
public:
	//初期化代入わからんので関数化
	void assign_RGB(){
		R.data = &data;
		R.color = 8;
		G.data = &data;
		G.color = 4;
		B.data = &data;
		B.color = 0;
	}

	RGB(){
		data = 0x0000;
		assign_RGB();
	}
	RGB(int data){
		this->data = data;
		assign_RGB();
	}
	RGB(int r, int g, int b){
		this->setR(r)->setG(g)->setB(b);
		assign_RGB();
	}

	//　プロパティもどきと添字演算子で楽したい
    RGB_prop R,G,B;
    RGB_prop operator[](int i){
		switch(i){
		case 0: return R;
		case 1: return G;
		case 2: return B;
		default:{
			RGB_prop null_dat;
			null_dat.data = (int *)0;
			null_dat.color = -1;
			return null_dat;
			}
		}
	}


	int operator()(){
		return data;
	}

    void operator()(int v){
    	data = v;
    }

    int getColor(){
    	return data;
    }
    void setColor(int color){
		data = color;
	}

	// getter setter 多分使わないかも
	int getR(){
		return (int)(data & 0x0f00 >> 8);
	}
	RGB* setR(int r){
		data &= 0xfffff0ff;
		data |= (r & 0x00f) << 8;
		return this;
	}
	int getG(){
		return (data & 0x00f0 >> 4);
	}
	RGB* setG(int g){
		data &= 0xffffff0f;
		data |= (g & 0x00f) << 4;
		return this;
	}
	int getB(){
		return (data & 0x000f);
	}
	RGB* setB(int b){
		data &= 0xfffffff0;
		data |= (b & 0x00f);
		return this;
	}
};


class RGB_DATALINE {
private:
	int length;
	std::vector<RGB> data_line;
public:
	//
	RGB_DATALINE(int);
	virtual ~RGB_DATALINE();
	// operator
	RGB operator[](int i);
	//

};

#endif /* SRC_RGBDATALINE_H_ */
