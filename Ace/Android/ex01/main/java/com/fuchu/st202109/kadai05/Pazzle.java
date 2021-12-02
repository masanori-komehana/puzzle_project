package com.fuchu.st202109.kadai05;


import java.util.Deque;
import java.util.Random;

public class Pazzle {
    public static Pazzle instance = new Pazzle();

    public static Pazzle getInstance() {
        return instance;
    }

    enum MoveDirection {
        MOVE_NO_MOVE,
        MOVE_LEFT,
        MOVE_DOWN,
        MOVE_UP,
        MOVE_RIGHT,
    }



    public static final int BOARD_SIZE = 4;
    private PazzlePiece[][] board = new PazzlePiece[BOARD_SIZE][BOARD_SIZE];
//    private static final int BLANK_NUMBER = 16;

    private GridPosition blankPos;

    private Pazzle() {
        boardInit();
    }

    public void boardInit(){
        Deque<PazzlePiece> pieces = PazzlePiece.getDeque();
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                board[i][j] = pieces.poll();
                if(board[i][j].isBlank()){
                    blankPos = new GridPosition(i, j);
                }
            }
        }
    }

    public boolean shuffleBoard(){
        Random r = new Random();
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
            return false;
        }
        PazzlePiece tempPiece = board[blankPos.getRow()][blankPos.getCol()];
        board[blankPos.getRow()][blankPos.getCol()] = board[p.getRow()][p.getCol()];
        board[p.getRow()][p.getCol()] = tempPiece;

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

    public PazzlePiece[][] getBoard() {
        return board;
    }
}
