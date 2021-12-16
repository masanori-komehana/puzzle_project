package com.fuchu.st202109.kadai05;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.util.List;
import java.util.Locale;

public class MainActivity extends AppCompatActivity {


    private DBHelper dbHelper = new DBHelper(this, null, null, 1);

    // TODO ランキングアクティビティ
    // TODO 統計アクティビティ
    // TODO

    private Activity act = this;
    private List<PlayerItem> list;
    private static final int SUB_ACTIVITY = 100;
    private static final int PAZZLE_ACTIVITY = 101;
    private static final int REGISTER_ACTIVITY = 102;


    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if(resultCode == RESULT_OK && requestCode == REGISTER_ACTIVITY){
            Toast.makeText(getApplicationContext(), dbHelper.getCurrentPlayer().getName()+"を選択しました。", Toast.LENGTH_SHORT).show();
            updateCurrentPlayerName();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final Activity act = this;


        Button btnPazzle = findViewById(R.id.btn_pazzle);
        btnPazzle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(act, PazzleActivity.class);
                act.startActivityForResult(intent, PAZZLE_ACTIVITY);
            }
        });





        Button btnRegister = findViewById(R.id.btn_name_config);
        btnRegister.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(act, RegisterActivity.class);
                act.startActivityForResult(intent, REGISTER_ACTIVITY);
            }
        });

        list = dbHelper.getPlayerAll();
        if(list.size() == 0){
            final EditText editText = new EditText(this);
            editText.setHint("プレイヤー名を入力");
            new AlertDialog.Builder(this)
                    .setTitle("プレイヤー登録")
                    .setMessage("15パズルAをインストールしていただき、ありがとうございます。\n" +
                            "本アプリではプレイヤー登録が必要です。\n" +
                            "名前を入力してください。")
                    .setView(editText)
                    .setPositiveButton("OK",null)
                    .setOnDismissListener(new DialogInterface.OnDismissListener() {
                        @Override
                        public void onDismiss(DialogInterface dialogInterface) {
                            String name = editText.getText().toString();
                            if(dbHelper.addPlayer(name)){
                                dbHelper.setCurrent(name);
                                new AlertDialog.Builder(act)
                                        .setTitle("登録完了")
                                        .setMessage("登録ありがとうございます。\n" +
                                                "それでは15パズルAをお楽しみください！")
                                        .setPositiveButton("OK", null)
                                        .show();
                            }else {
                                new AlertDialog.Builder(act)
                                        .setTitle("登録失敗")
                                        .setMessage("登録に失敗しました\n" +
                                                "アプリを再起動して\n" +
                                                "もう一度やり直してください")
                                        .setPositiveButton("OK", null)
                                        .setOnDismissListener(new DialogInterface.OnDismissListener() {
                                            @Override
                                            public void onDismiss(DialogInterface dialogInterface) {
                                                finish();
                                            }
                                        })
                                        .show();
                            }
                        }
                    })
                    .show();
        }

        updateCurrentPlayerName();
    }


    public void updateCurrentPlayerName(){
        TextView text_current_pname = findViewById(R.id.text_current_pname);
        PlayerItem currentPlayer = dbHelper.getCurrentPlayer();
        if(currentPlayer != null) {
            text_current_pname.setText(currentPlayer.getName());
        }else{
            list = dbHelper.getPlayerAll();
            if(list.size() == 1){
                text_current_pname.setText(list.get(0).getName());
            }else if(list.size() > 1){
                //TODO 選ばせるダイアログ
            }
        }
    }
}

/*
#ff0000 #ffbe00
#ff6000 #e1ff00
#ffbf00 #81ff00
#dfff00 #22ff00
#80ff00 #00ff3e
#20ff00 #00ff9e
#00ff40 #00fffd
#00ff9f #00a1ff
#00ffff #0041ff
#009fff #1e00ff
#0040ff #7e00ff
#2000ff #dd00ff
#8000ff #ff00c1
#df00ff #ff0061
#ff00bf #ff0002
#ff0060 #ff5e00
 */

