package com.fuchu.st202109.kadai05;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class PazzleSurfaceGraphic extends SurfaceView implements SurfaceHolder.Callback {

    private Bitmap backBmp;
    private int gridSize = 0;
    private float pieceSize = 0f;
    private Bitmap[] pieceBitmaps = new Bitmap[16];
    private float pointX = 0f, pointY= 0f;
    private GridPosition selectedPositon = new GridPosition(-1, -1);
    private RectF[][] pieceRects = new RectF[Pazzle.BOARD_SIZE][Pazzle.BOARD_SIZE];
    private final Pazzle pazzle = Pazzle.getInstance();

    private ScheduledExecutorService drawExec;
    private final class DrawTask implements Runnable {
        SurfaceHolder surfaceHolder;
        Bitmap backBmp;

        public DrawTask(SurfaceHolder surfaceHolder){
            this.surfaceHolder = surfaceHolder;
        }

        @Override
        public void run() {

            Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);

            Canvas canvas = surfaceHolder.lockCanvas();
            canvas.drawColor(Color.WHITE);
            paint.setColor(Color.BLACK);
            drawBoardPieces(canvas, paint);

            if (backBmp != null) {
                canvas.drawBitmap(backBmp, 0f, 0f, paint);
            } else {
                backBmp = Bitmap.createBitmap(canvas.getWidth(), canvas.getHeight(), Bitmap.Config.ARGB_8888);
            }
            surfaceHolder.unlockCanvasAndPost(canvas);
        }
    }

    public PazzleSurfaceGraphic(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        getHolder().addCallback(this);
    }


    @Override
    public void surfaceCreated(@NonNull SurfaceHolder surfaceHolder) {
        drawExec = Executors.newSingleThreadScheduledExecutor();
        drawExec.scheduleAtFixedRate(new DrawTask(surfaceHolder), 0, 10, TimeUnit.MILLISECONDS);
    }


    @Override
    public void surfaceChanged(@NonNull SurfaceHolder surfaceHolder, int i, int i1, int i2) {
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder surfaceHolder) {

    }

    @SuppressLint("DrawAllocation")
    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        super.onLayout(changed, left, top, right, bottom);

        gridSize = getWidth() / Pazzle.BOARD_SIZE;
        pieceSize = (float) getWidth() / Pazzle.BOARD_SIZE;

        for (PazzlePiece p : PazzlePiece.values()){
            if(!p.isBlank())
            pieceBitmaps[p.getNum()] = BitmapFactory.decodeResource(getResources(), p.getId());
        }

        for (int i = 0; i < Pazzle.BOARD_SIZE; i++) {
            for (int j = 0; j < Pazzle.BOARD_SIZE; j++) {
                pieceRects[i][j] = new RectF(j * pieceSize,
                    i * pieceSize,
                    (j+1) * pieceSize,
                    (i+1) * pieceSize
                );
            }
        }
    }

    public RectF getPieceRect(GridPosition p) {
        return pieceRects[p.getRow()][p.getCol()];
    }

    //    public void drawBoard(Canvas canvas, Paint paint){
//        for (int i = 0; i < Pazzle.BOARD_SIZE; i++) {
//            for (int j = 0; j < Pazzle.BOARD_SIZE; j++) {
//                RectF r = pieceRects[i][j];
//
//            }
//        }
//    }

    public void drawPiece(int col, int row, Bitmap bmp, Canvas canvas, Paint paint) {
        Rect srcRect = new Rect(0, 0, bmp.getWidth(), bmp.getHeight());
        RectF destRect = pieceRects[row][col];
        canvas.drawBitmap(bmp, srcRect, destRect, paint);
    }

    public void drawBoardPieces(Canvas canvas, Paint paint) {
        Bitmap frame = BitmapFactory.decodeResource(getResources(), R.drawable.button);

        GridPosition currentPosition = new GridPosition();
        final int green500 = (getResources().getColor(R.color.green_500));
        final int red500   = (getResources().getColor(R.color.red_500));

        for (int i = 0; i < Pazzle.BOARD_SIZE; i++) {
            currentPosition.setRow(i);
            for (int j = 0; j < Pazzle.BOARD_SIZE; j++) {
                Bitmap bmp;
                PazzlePiece p;
                currentPosition.setCol(j);
                p = pazzle.getBoardPiece(currentPosition);
                if (pazzle.isActive() && pazzle.canMove(currentPosition)){
                    paint.setColor(green500);
                    canvas.drawRect(pieceRects[i][j], paint);
                    paint.setColor(Color.BLACK);
                } else {
                    canvas.drawRect(pieceRects[i][j], paint);
                }
                if (p.isBlank())continue;
                bmp = pieceBitmaps[pazzle.getBoardPiece(currentPosition).getNum()];
                drawPiece(j, i, frame, canvas, paint);
                drawPiece(j, i, bmp, canvas, paint);
            }
        }
    }

    @SuppressLint({"CanvasSize", "DrawAllocation"})
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

    }

    public void touch(int action, GridPosition p) {
        // TODO ピース選ぶやつ
        switch (action) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_MOVE:
                selectedPositon = p;
                break;
            case MotionEvent.ACTION_UP:
                selectedPositon.setRow(-1).setCol(-1);
                break;
            default:
        }
        invalidate();
    }
}
