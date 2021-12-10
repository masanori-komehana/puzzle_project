package com.fuchu.st202109.kadai05;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.util.Locale;

public class MainActivity extends AppCompatActivity {

    // TODO プレイヤー見つからないときに登録させる処理。
    // TODO ランキングアクティビティ
    // TODO 統計アクティビティ
    // TODO

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Activity act = this;

        Button btnPazzle = findViewById(R.id.btn_pazzle);
        btnPazzle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(act, PazzleActivity.class);
                act.startActivity(intent);
            }
        });


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

