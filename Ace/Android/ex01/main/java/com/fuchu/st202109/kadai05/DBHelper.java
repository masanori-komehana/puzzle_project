package com.fuchu.st202109.kadai05;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import androidx.annotation.Nullable;

public class DBHelper extends SQLiteOpenHelper {
    private final static String DB_NAME = "pazzle15Ace.db";
    private final static int DB_VERSION = 1;

    public void newPlayer(String name){
        SQLiteDatabase db = getWritableDatabase();
        db.execSQL("insert into player (name) values ("+name+");");
        db.close();
    }

    public DBHelper(@Nullable Context context, @Nullable String name, @Nullable SQLiteDatabase.CursorFactory factory, int version) {
        super(context, DB_NAME, factory, DB_VERSION);
    }


    @Override
    public void onCreate(SQLiteDatabase db) {
        String query = "create table result(" +
                "id integer PRIMARY KEY AUTOINCREMENT," +
                "player_id integer NOT NULL," +
                "pazzletime integer NOT NULL," +
                "movecount integer NOT NULL," +
                "playdatetime text DEFAULT CURRENT_TIMESTAMP," +
                "resigned integer DEFAULT 0" +
                ");";
        db.execSQL(query);


        query = "create table player(" +
                "player_id integer PRIMARY KEY AUTOINCREMENT," +
                "name text NOT NULL," +
                "created_dt text DEFAULT CURRENT_TIMESTAMP," +
                "current integer DEFAULT 0" +
                ");";
        db.execSQL(query);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {

    }


}

