package com.fuchu.st202109.kadai05;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

public class PazzleActivity extends AppCompatActivity {

    PazzleGraphic pazzleGraphic;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pazzle);

        pazzleGraphic = findViewById(R.id.pazzle_graphic);

        pazzleGraphic.invalidate();
    }
}