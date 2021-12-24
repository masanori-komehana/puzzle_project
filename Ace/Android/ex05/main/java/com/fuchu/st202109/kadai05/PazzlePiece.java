package com.fuchu.st202109.kadai05;

import android.app.Activity;
import android.graphics.drawable.Drawable;

import java.util.ArrayDeque;
import java.util.Arrays;
import java.util.Deque;
import java.util.stream.Collectors;

public enum PazzlePiece {
    num01(1, R.drawable.num1),
    num02(2, R.drawable.num2),
    num03(3, R.drawable.num3),
    num04(4, R.drawable.num4),
    num05(5, R.drawable.num5),
    num06(6, R.drawable.num6),
    num07(7, R.drawable.num7),
    num08(8, R.drawable.num8),
    num09(9, R.drawable.num9),
    num10(10, R.drawable.num10),
    num11(11, R.drawable.num11),
    num12(12, R.drawable.num12),
    num13(13, R.drawable.num13),
    num14(14, R.drawable.num14),
    num15(15, R.drawable.num15),
    num16(16, 0){
        @Override
        public boolean isBlank() {
            return true;
        }

//        @Override
//        public Drawable getDrawable(Activity act) {
//            return null;
//        }
    },
    ;

    private final int num;
    private final int id;

    private PazzlePiece(int num, int id){
        this.num = num;
        this.id = id;
    }

    public int getId() {
        return id;
    }

    public int getNum() {
        return num;
    }

    public boolean isBlank(){
        return false;
    }

//    public Drawable getDrawable(Activity act){
//        return act.getResources().getDrawable(id);
//    }

    public static Deque<PazzlePiece> getDeque(){
        Deque<PazzlePiece> pieces = new ArrayDeque<>(PazzlePiece.values().length);
        pieces.addAll(Arrays.stream(PazzlePiece.values()).collect(Collectors.toList()));
        return pieces;
    }

}
