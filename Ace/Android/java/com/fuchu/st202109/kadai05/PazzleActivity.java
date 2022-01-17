package com.fuchu.st202109.kadai05;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.ContentValues;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.RectF;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.util.TypedValue;
import android.view.MenuItem;
import android.view.MotionEvent;
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

    DBHelper dbHelper = new DBHelper(this, null, null, 1);

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

    private int pazzleTime;
    private int movecount = 0;
    private int regined = 0;
    private ContentValues values = new ContentValues();


    private int time10ms;
    private int moveCnt;

    private Handler updateTextHandler = new Handler();


    public String time10ms_toString(int t){
        return String.format(Locale.JAPAN,
                "%02d:%02d.%02d",
                t/6000,
                (t/100)%60,
                t%100);
    }

    private ScheduledExecutorService exec = null;
    private final Runnable task = new Runnable() {
        @Override
        public void run() {
//            if(pazzleGraphic != null) pazzleGraphic.postInvalidate();
//            if (pazzleGraphic != null) pazzleGraphic.reqDraw();
            if(pazzle.isActive() && !pazzle.isPazzleClear()) {
                time10ms++;
                pazzle.setPazzletime(time10ms);
                updateTextHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        timeText.setText(time10ms_toString(time10ms));
                    }
                });
            }
        }
    };

    private int shuffleNum = 1000;
    private int shuffleCnt = 0;
    private ScheduledExecutorService shuffleExec;


    private boolean flg_isShuffleNow = false;
    private Handler shuffleHandler = new Handler();

    private final class ShuffleTask implements Runnable {

        @Override
        public void run() {
            shuffleHandler.post(new Runnable() {
                @Override
                public void run() {
                    if (shuffleCnt < shuffleNum){
                        pazzle.shuffleBoard();
//                        Log.i("ShuffleTask", (shuffleCnt)+"/"+shuffleNum);
//                        if(shuffleCnt % 10 == 0)pazzleGraphic.postInvalidate();
                        pazzleGraphic.reqDraw();
                        shuffleCnt++;
                    }else {
                        shuffleCnt = pazzle.getMoved();
                        startPazzle();
                    }
                }
            });
        }
    }
    
    public boolean isShuffleNow(){
        return flg_isShuffleNow;
    }



    public void startPazzle(){

        shuffleExec.shutdown();
        flg_isShuffleNow = false;

        new AlertDialog.Builder(act)
                .setTitle("パズル開始")
                .setNeutralButton("OK", null)
                .setOnDismissListener(new DialogInterface.OnDismissListener() {
                    @Override
                    public void onDismiss(DialogInterface dialogInterface) {
                        // TODO
                        pazzle.activate();
                        moveCnt = 0;
                        time10ms = 0;
                        btn.setEnabled(true);
                        btn.setBackgroundColor(getResources().getColor(R.color.red_500));
                        btn.setText(getResources().getString(R.string.laben_btn_retire));

                    }
                })
                .show();
    }

    private void pazzleCleared() {
        pazzleTime = time10ms;
        movecount = moveCnt;
        timeText.setText(time10ms_toString(pazzleTime));
        new AlertDialog.Builder(act)
                .setTitle("パズル完成")
                .setMessage(String.format(Locale.JAPAN,
                        "記録:\n" +
                                "\t 🕒:%s\n"+
                                "\t 手数:%03d",
                        time10ms_toString(pazzleTime),
                        movecount))
                .setNeutralButton("OK", null)
                .setOnDismissListener(new DialogInterface.OnDismissListener() {
                    @Override
                    public void onDismiss(DialogInterface dialogInterface) {
                        regined = 0;
                        pazzle.setPazzletime(pazzleTime);
                        pazzle.setMoved(movecount);
                        pazzleInactivate();
                        sendSQL();
                    }
                })
                .show();
    }

    private void sendSQL() {
        int player_id = dbHelper.getCurrentPlayer().getPlayerId();
        dbHelper.addResult(player_id, pazzleTime, movecount, regined);
        ResultItem resultItem = dbHelper.getResultLatest();
        Toast.makeText(getApplicationContext(), "プレイ結果を登録しました。\n" +
                "名前："+resultItem.getPlayerName(this)+"\n"+
                "時間："+resultItem.getMovecount()+"\n"+
                "手数："+resultItem.getPazzleTimeStr(), Toast.LENGTH_LONG).show();
        pazzle.inactivate();
        pazzle.boardInit();
    }

    @Override
    protected void onDestroy() {
        if(pazzle.isActive()){
            pazzle.setPazzletime(time10ms);
            pazzle.setMoved(movecount);
            pazzleActReset();
        }
        super.onDestroy();
    }


    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {

        int id = item.getItemId();
        if(id == android.R.id.home){
            Intent intent = new Intent();
            setResult(RESULT_OK, intent);
            finish();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pazzle);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        squareLayout = findViewById(R.id.square_layout);

        TypedValue value = new TypedValue();
        if (getTheme().resolveAttribute(android.R.attr.actionBarSize, value, true)) {
            actionHeight = TypedValue.complexToDimensionPixelSize(value.data,
                    getResources().getDisplayMetrics());
        }

        timeText = findViewById(R.id.text_pazzle_time);
        moveCntText = findViewById(R.id.text_move_cnt);

        pazzleGraphic = findViewById(R.id.pazzle_graphic);
//        pazzleGraphic.invalidate();
        pazzleGraphic.reqDraw();

        btn = findViewById(R.id.btn_retire);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (!pazzle.isActive() && !flg_isShuffleNow) {
                    flg_isShuffleNow = true;
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
                            .setMessage("諦めますか？")
                            .setPositiveButton("はい", new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialogInterface, int i) {
                                    // YES button pressed
                                    pazzleResign();
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

    private void pazzleResign() {
        regined = 1;
        pazzleTime = time10ms;
        movecount = moveCnt;
        pazzle.setPazzletime(pazzleTime);
        pazzle.setMoved(movecount);
        pazzleInactivate();
        sendSQL();
    }

    private void pazzleInactivate() {
        pazzle.inactivate();
        pazzleActReset();
    }

    private void pazzleActReset() {
        btn.setBackgroundColor(getResources().getColor(R.color.blue_500));
        btn.setText("スタート");
        timeText.setText(getResources().getString(R.string.time_zero));
        moveCntText.setText(getResources().getString(R.string.move_zero));
        pazzle.boardInit();
    }

    @Override
    protected void onResume() {
        super.onResume();
        exec = Executors.newSingleThreadScheduledExecutor();
        exec.scheduleAtFixedRate(task, 0, 10, TimeUnit.MILLISECONDS);
        pazzleGraphic.reqDraw();
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
            case MotionEvent.ACTION_MOVE:
            case MotionEvent.ACTION_DOWN:
                pointX = event.getX();
                pointY = event.getY() - actionHeight - rayoutTop;
                if (pazzle.isActive()) {
                    touchSelectedPosition();
                    if (pazzle.canMove(pos)){
                        pazzle.moveBlank(pos);
//                        pazzleGraphic.invalidate()
                        pazzleGraphic.reqDraw();
                        moveCnt++;
                        pazzle.setMoved(moveCnt);
                        moveCntText.setText(String.format(Locale.JAPAN,
                                "%03d手", moveCnt));
                        if (pazzle.isPazzleClear()){
                            pazzleGraphic.reqDraw();
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

