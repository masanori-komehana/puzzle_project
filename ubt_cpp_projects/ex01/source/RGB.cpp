#include "RGB.h"


int dbg_FLG = 1;


//初期化代入わからんので関数化
void RGB::assign_RGB(){
    R.data = &data;
    R.color = 8;
    G.data = &data;
    G.color = 4;
    B.data = &data;
    B.color = 0;
}

RGB::RGB(){
    data = 0x0000;
    assign_RGB();
}
RGB::RGB(int data){
    this->data = data;
    assign_RGB();
}
RGB::RGB(int r, int g, int b){
    this->setR(r)->setG(g)->setB(b);
    assign_RGB();
}


RGB_prop RGB::operator[](int i){
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


int RGB::operator()(){
    return this->data;
}

void RGB::operator()(int v){
    this->data = v;
}

int RGB::getColor(){
    return data;
}
void RGB::setColor(int color){
    if(color != 0 && dbg_FLG){
        cout <<setw(3)<<setfill('0')<< this->data << endl;
    }
    this->data = color;
    if(color != 0 && dbg_FLG){
        dbg_FLG = 0;
        cout <<setw(3)<<setfill('0')<< this->data << endl;
    }
}

// getter setter 多分使わないかも
int RGB::getR(){
    return (int)(data & 0x0f00 >> 8);
}
RGB* RGB::setR(int r){
    data &= 0xfffff0ff;
    data |= (r & 0x00f) << 8;
    return this;
}
int RGB::getG(){
    return (data & 0x00f0 >> 4);
}
RGB* RGB::setG(int g){
    data &= 0xffffff0f;
    data |= (g & 0x00f) << 4;
    return this;
}
int RGB::getB(){
    return (data & 0x000f);
}
RGB* RGB::setB(int b){
    data &= 0xfffffff0;
    data |= (b & 0x00f);
    return this;
}
