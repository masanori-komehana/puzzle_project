package com.fuchu.st202109.kadai05;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.LinearLayout;

public class SquareLayout extends LinearLayout {

    int size;

    public SquareLayout(Context context) {
        super(context);
    }

    public SquareLayout(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        size = widthMeasureSpec;
        super.onMeasure(size, size);
    }
}