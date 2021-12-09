package com.fuchu.st202109.kadai05;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.util.TypedValue;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.ViewTreeObserver;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Locale;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class PazzleActivity extends AppCompatActivity {

    private final Activity act = this;
    private PazzleSurfaceGraphic pazzleGraphic;
    private SquareLayout squareLayout;

    private TextView timeText;
    private TextView moveCntText;

    private GridPosition pos = new GridPosition();
    private float actionHeight = 0f;
    private float rayoutTop = 0f;
    private float pointX;
    private float pointY;
    private Button btn;

    private Pazzle pazzle = Pazzle.getInstance();


    private int time10ms = -1;
    private int moveCnt = 0;


    private Handler updateTextHandler = new Handler();


    public String time10ms_toString(int t){
        return String.format(Locale.JAPAN,
                "%02d:%02d.%02d",
                time10ms/6000,
                (time10ms/100)%60,
                time10ms%100);
    }

    private ScheduledExecutorService exec = null;
    private final Runnable task = new Runnable() {
        @Override
        public void run() {
            if(pazzleGraphic != null) pazzleGraphic.postInvalidate();
            if(pazzle.isActive() && !pazzle.isPazzleClear()) {
                time10ms++;
                updateTextHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        timeText.setText(time10ms_toString(time10ms));
                    }
                });
            } else {
                time10ms = -1;
                updateTextHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        timeText.setText("00:00.00");
                    }
                });
            }
        }
    };

    private int shuffleNum = 1000;
    private int shuffleCnt = 0;
    private ScheduledExecutorService shuffleExec;


    private boolean isShuffleNow = false;
    private Handler shuffleHandler = new Handler();

    private final class ShuffleTask implements Runnable {

        @Override
        public void run() {
            shuffleHandler.post(new Runnable() {
                @Override
                public void run() {
                    if (shuffleCnt < shuffleNum){
                        pazzle.shuffleBoard();
                        if(shuffleCnt % 10 == 0)pazzleGraphic.postInvalidate();
                        shuffleCnt++;
                    }else {
                        shuffleCnt = 0;
                        startPazzle();
                    }
                }
            });
        }
    }



    public void startPazzle(){
        isShuffleNow = false;
        shuffleExec.shutdown();

        new AlertDialog.Builder(act)
                .setTitle("パズル開始")
                .setNeutralButton("OK", null)
                .setOnDismissListener(new DialogInterface.OnDismissListener() {
                    @Override
                    public void onDismiss(DialogInterface dialogInterface) {
                        // TODO
                        pazzle.activate();
                        moveCnt = 0;
                        btn.setEnabled(true);
                        btn.setBackgroundColor(getResources().getColor(R.color.red_500));
                        btn.setText(getResources().getString(R.string.laben_btn_retire));

                    }
                })
                .show();
    }

    private void pazzleCleared() {
        int pazzleTime = time10ms;
        int moved = moveCnt;
        new AlertDialog.Builder(act)
                .setTitle("パズル完成")
                .setMessage(String.format(Locale.JAPAN,
                        "記録:\n" +
                                "\t 🕒:%s\n"+
                                "\t 手数:%03d",
                        time10ms_toString(pazzleTime),
                        moved))
                .setNeutralButton("OK", null)
                .setOnDismissListener(new DialogInterface.OnDismissListener() {
                    @Override
                    public void onDismiss(DialogInterface dialogInterface) {
                        pazzleInactivate();
                        sendSQL();
                    }
                })
                .show();
    }

    private void sendSQL() {
        // TODO SQLite投げる処理
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pazzle);

        squareLayout = findViewById(R.id.square_layout);

        TypedValue value = new TypedValue();
        if (getTheme().resolveAttribute(android.R.attr.actionBarSize, value, true)) {
            actionHeight = TypedValue.complexToDimensionPixelSize(value.data,
                    getResources().getDisplayMetrics());
        }

        timeText = findViewById(R.id.text_pazzle_time);
        moveCntText = findViewById(R.id.text_move_cnt);

        pazzleGraphic = findViewById(R.id.pazzle_graphic);
        pazzleGraphic.invalidate();

        btn = findViewById(R.id.btn_retire);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (!pazzle.isActive() && !isShuffleNow) {
                    isShuffleNow = true;
                    Toast.makeText(act, getResources().getString(R.string.label_onshuffle), Toast.LENGTH_SHORT).show();
                    btn.setText(getResources().getString(R.string.label_onshuffle));
                    btn.setBackgroundColor(getResources().getColor(R.color.bluegray_500));
                    btn.setEnabled(false);
                    shuffleNum = 1000;
                    shuffleCnt = 0;
                    shuffleExec = Executors.newSingleThreadScheduledExecutor();
                    shuffleExec.scheduleAtFixedRate(new ShuffleTask(),0, 3, TimeUnit.MILLISECONDS);
                }else if(pazzle.isActive()){
                    new AlertDialog.Builder(act)
                            .setTitle("確認")
                            .setMessage("このパズルを諦めますか？")
                            .setPositiveButton("はい", new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialogInterface, int i) {
                                    // YES button pressed
                                    pazzleInactivate();
                                    sendSQL();
                                }
                            })
                            .setNegativeButton("いいえ",null)
                            .show();
                }
            }
        });
        btn.setBackgroundColor(getResources().getColor(R.color.blue_500));
        btn.setText("スタート");

        ViewTreeObserver observer = squareLayout.getViewTreeObserver();
        observer.addOnGlobalLayoutListener(
                new ViewTreeObserver.OnGlobalLayoutListener() {
                    @Override
                    public void onGlobalLayout() {
                        rayoutTop = squareLayout.getTop();
                    }
                });

    }

    private void pazzleInactivate() {
        pazzle.inactivate();
        btn.setBackgroundColor(getResources().getColor(R.color.blue_500));
        btn.setText("スタート");
        moveCntText.setText("000手");
        pazzle.boardInit();
    }

    @Override
    protected void onResume() {
        super.onResume();
        exec = Executors.newSingleThreadScheduledExecutor();
        exec.scheduleAtFixedRate(task, 0, 10, TimeUnit.MILLISECONDS);


    }

    public void touchSelectedPosition(){

        GridPosition currentPosition = new GridPosition();

        pos.setCol(-1).setRow(-1);

        for (int i = 0; i < Pazzle.BOARD_SIZE; i++) {
            currentPosition.setRow(i);
            for (int j = 0; j < Pazzle.BOARD_SIZE; j++) {
                currentPosition.setCol(j);
                RectF rectF = pazzleGraphic.getPieceRect(currentPosition);
                if (rectF.contains(pointX, pointY)){
                    pos.setRow(i).setCol(j);
                }
            }
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_MOVE:
                pointX = event.getX();
                pointY = event.getY() - actionHeight - rayoutTop;
                if (pazzle.isActive()) {
                    touchSelectedPosition();
                    if (pazzle.canMove(pos)){
                        pazzle.moveBlank(pos);
                        pazzleGraphic.invalidate();
                        moveCnt++;
                        moveCntText.setText(String.format(Locale.JAPAN,
                                "%03d手", moveCnt));
                        if (pazzle.isPazzleClear()){
                            pazzleCleared();
                        }
                    }
                }
                break;
            default:
        }
        pazzleGraphic.touch(event.getAction(), pos);
        if (!pos.equals(GridPosition.None)) return true;
        return super.onTouchEvent(event);
    }

}