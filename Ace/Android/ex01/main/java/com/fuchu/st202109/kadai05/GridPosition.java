package com.fuchu.st202109.kadai05;

import androidx.annotation.NonNull;

// マスの位置
public class GridPosition {
    public static final GridPosition None = new GridPosition(-1, -1, true);

    private int row;
    private int col;
    private boolean isConst = false;

    public GridPosition(int row, int col) {
        this.row = row;
        this.col = col;
    }

    private GridPosition(int row, int col, boolean isConst) {
        this.row = row;
        this.col = col;
        this.isConst = isConst;
    }

    public GridPosition(@NonNull GridPosition s) {
        this.row = s.row;
        this.col = s.col;
        this.isConst = s.isConst;
    }

    // clone使い方わからん
    public GridPosition copy(){
        return new GridPosition(this);
    }

    public int getCol() {
        return col;
    }

    public int getRow() {
        return row;
    }

    public GridPosition move(int r, int c){
        GridPosition movedPos = this.copy();
        movedPos.setRow(row + r).setCol(col + c);
        return movedPos;
    }

    public GridPosition setCol(int col) {
        if(isConst)return this;
        this.col = col;
        return this;
    }

    public GridPosition setRow(int row) {
        if(isConst)return this;
        this.row = row;
        return this;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        GridPosition that = (GridPosition) o;

        if (row != that.row) return false;
        return col == that.col;
    }

    @Override
    public int hashCode() {
        int result = row;
        result = 31 * result + col;
        return result;
    }
}
