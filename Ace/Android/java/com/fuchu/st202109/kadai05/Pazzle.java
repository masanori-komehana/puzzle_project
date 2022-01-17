package com.fuchu.st202109.kadai05;


import java.util.Deque;
import java.util.Random;

public class Pazzle {
    public static final int BOARD_SIZE = 4;
    private PazzlePiece[][] board = new PazzlePiece[BOARD_SIZE][BOARD_SIZE];

    private final Random r = new Random();
    //    private static final int BLANK_NUMBER = 16;
    private int pazzletime = 0;
    private int moved = 0;

    private boolean flg_Active = false;


    enum MoveDirection {
        MOVE_NO_MOVE,
        MOVE_LEFT,
        MOVE_DOWN,
        MOVE_UP,
        MOVE_RIGHT,
    }


    public static Pazzle instance = new Pazzle();

    public static Pazzle getInstance() {
        return instance;
    }

    public boolean isActive() {
        return flg_Active;
    }

    public void activate(){
        flg_Active = true;
    }

    public void inactivate(){
        flg_Active = false;
    }

    public int getPazzletime() {
        return pazzletime;
    }

    public void setPazzletime(int pazzletime) {
        this.pazzletime = pazzletime;
    }

    public int getMoved() {
        return moved;
    }

    public void setMoved(int moved) {
        this.moved = moved;
    }


    private GridPosition blankPos;

    private Pazzle() {
        boardInit();
    }

    public void boardInit(){
        Deque<PazzlePiece> pieces = PazzlePiece.getDeque();
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                board[i][j] = pieces.poll();
                assert board[i][j] != null;
                if(board[i][j].isBlank()){
                    blankPos = new GridPosition(i, j);
                }
            }
        }
    }

    public boolean shuffleBoard(){
        MoveDirection[] directions = {
            MoveDirection.MOVE_LEFT,
            MoveDirection.MOVE_DOWN,
            MoveDirection.MOVE_UP,
            MoveDirection.MOVE_RIGHT,
        };
        return pazzleMove(directions[r.nextInt(directions.length)]);
    }


    public boolean pazzleMove(MoveDirection direction){
        GridPosition movePos;
        switch (direction){
            case MOVE_LEFT:
                movePos = blankPos.move(0, 1);
                break;
            case MOVE_DOWN:
                movePos = blankPos.move(-1, 0);
                break;
            case MOVE_UP:
                movePos = blankPos.move(1, 0);
                break;
            case MOVE_RIGHT:
                movePos = blankPos.move(0, -1);
                break;
            default:
                return false;
        }
        return moveBlank(movePos);
    }

    public boolean moveBlank(GridPosition p){
        if(!canMove(p)){
//            Log.i("moveBlank", String.format(Locale.JAPAN,
//                    "Pos(%d,%d) is not able to move", p.getRow(), p.getCol()));
            return false;
        }
        PazzlePiece tempPiece = board[blankPos.getRow()][blankPos.getCol()];
        board[blankPos.getRow()][blankPos.getCol()] = board[p.getRow()][p.getCol()];
        board[p.getRow()][p.getCol()] = tempPiece;
        blankPos = p.copy();

        return true;
    }

    public boolean canMove(GridPosition p){
        if (p.getCol() < 0
            || p.getCol() >= BOARD_SIZE
            || p.getRow() < 0
            || p.getRow() >= BOARD_SIZE)
        {
            return false;
        }

        if(p.equals(blankPos)){
            return false;
        }

        GridPosition[] positions = {
            blankPos.move(0, 1),
            blankPos.move(-1, 0),
            blankPos.move(1, 0),
            blankPos.move(0, -1),
        };

        for (GridPosition pos: positions) {
            if (pos.getCol() < 0
                || pos.getCol() >= BOARD_SIZE
                || pos.getRow() < 0
                || pos.getRow() >= BOARD_SIZE)
            {
                continue;
            }
            if(p.equals(pos)){
                return true;
            }
        }

        return false;
    }

    public boolean isPazzleClear(){
        Deque<PazzlePiece> pieces = PazzlePiece.getDeque();
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if(!board[i][j].equals(pieces.poll())){
                    return false;
                }
            }
        }
        return true;
    }

//    public PazzlePiece[][] getBoard() {
//        return board;
//    }

    public PazzlePiece getBoardPiece(GridPosition p) {
        return board[p.getRow()][p.getCol()];
    }
}
