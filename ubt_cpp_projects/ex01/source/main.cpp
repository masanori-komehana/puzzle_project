#include <vector>
#include <iostream>

#include "pazzle_project.h"
#include "LEDMAT.h"

using namespace std;

LEDMAT led_mat(32);
void test_cp_ledmat(void);

int main(void)
{
    test_cp_ledmat();
    return 0;
}

void test_cp_ledmat(){
	static int font1_data[4][8]={
		{0x20,0x7c,0x20,0x3c,0x6a,0xb2,0x64,0x0},
		{0x0,0x88,0x84,0x82,0x82,0x50,0x20,0x0},
		{0x78,0x0,0x78,0x84,0x4,0x8,0x70,0x0},
		{0x78,0x0,0xf8,0x10,0x30,0x50,0x8c,0x0}
	};
	static int font2_data[4][8]={
		{0xfc,0x84,0x84,0xfc,0x84,0x84,0xfc,0x0},
		{0x3e,0x22,0x3e,0x22,0x3e,0x42,0x86,0x0},
		{0x10,0x10,0x54,0x58,0x10,0x28,0xc6,0x0},
		{0x10,0x12,0xf4,0x38,0x54,0x92,0x30,0x0}
	};
	static int font3_data[4][8]={
		{0x10,0xfe,0x10,0x38,0x54,0x92,0x10,0x0},
		{0x10,0x28,0xfe,0x10,0xfe,0x54,0xfe,0x0},
		{0x10,0x10,0x7c,0x10,0x10,0x10,0xfe,0x0},
		{0x28,0x7e,0xc8,0x5c,0x5c,0x6a,0x48,0x0}
	};
	static int font4_data[4][8]={
		{0x8,0x7e,0x52,0x5e,0x72,0x5a,0x96,0x0},
		{0x10,0xfe,0x92,0x92,0xfe,0x10,0x10,0x0},
		{0x5c,0x94,0x5c,0xb4,0x5c,0xd4,0x7e,0x0},
		{0x98,0x48,0x8,0xd4,0x62,0x40,0xbe,0x0}
	};
	static int data[4][8];
	static int color[4] = {
		0x300, 0x030, 0x003, 0x333
	};
	static char mes[32][36];
	for (int i = 0; i < 8; ++i) {
		data[0][i]  = 0x00000000;
		data[0][i] |=(font1_data[0][i])<<24;
		data[0][i] |=(font1_data[1][i])<<16;
		data[0][i] |=(font1_data[2][i])<<8;
		data[0][i] |=(font1_data[3][i]);

		data[1][i]  = 0x00000000;
		data[1][i] |=(font2_data[0][i])<<24;
		data[1][i] |=(font2_data[1][i])<<16;
		data[1][i] |=(font2_data[2][i])<<8;
		data[1][i] |=(font2_data[3][i]);

		data[2][i]  = 0x00000000;
		data[2][i] |=(font3_data[0][i])<<24;
		data[2][i] |=(font3_data[1][i])<<16;
		data[2][i] |=(font3_data[2][i])<<8;
		data[2][i] |=(font3_data[3][i]);

		data[3][i]  = 0x00000000;
		data[3][i] |=(font4_data[0][i])<<24;
		data[3][i] |=(font4_data[1][i])<<16;
		data[3][i] |=(font4_data[2][i])<<8;
		data[3][i] |=(font4_data[3][i]);
	}
	for (int i = 0; i < 4; ++i) {
		led_mat.cpMat(8*i, 0, 32, 8, color[i], data[i]);
	}
	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32; ++j) {
			cout<<hex<<setw(3)<<setfill('0')<<led_mat[i][j]();
			cout<<',';
		}
		mes[i][32] = 0;
        cout << endl;
	}
	mes[31][33] = mes[31][33] + 0;
}
