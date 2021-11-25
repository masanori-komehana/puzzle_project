#ifndef SRC_RGB_H_
#define SRC_RGB_H_


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
private:
	int data;

public:

    void assign_RGB();

    RGB();
    RGB(int data);
    RGB(int r, int g, int b);

    //　プロパティもどきと添字演算子で楽したい
    RGB_prop R,G,B;
    RGB_prop operator[](int i);

	int operator()();

    void operator()(int v);

    int getColor();
    void setColor(int color);

	// getter setter 多分使わないかも
	int getR();
	RGB* setR(int r);
	int getG();
	RGB* setG(int g);
	int getB();
	RGB* setB(int b);
};

#endif /* end of include guard: SRC_RGB_H_ */
