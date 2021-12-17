package com.fuchu.st202109.kadai05;

import android.annotation.SuppressLint;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

import androidx.annotation.Nullable;

import java.util.List;

public class DBHelper extends SQLiteOpenHelper {
    private final static String DB_NAME = "pazzle15Ace.db";
    private final static int DB_VERSION = 1;

    //**********************************************************************************************
    // StatisticsItem関係
    //**********************************************************************************************
    public final StatisticsItem.Query statisticsItemQuery = new StatisticsItem.Query(this);


    //**********************************************************************************************
    // ResultItem関係
    //**********************************************************************************************

    public final ResultItem.Query resultItemQuery = new ResultItem.Query(this);


    public boolean addResult(int player_id, int pazzletime, int movecount, int resigned){
        return resultItemQuery.add(player_id, pazzletime, movecount, resigned);
    }

    //**********************************************************************************************
    // PlayerItem関係
    //**********************************************************************************************
    public final PlayerItem.Query playerItemQuery = new PlayerItem.Query(this);

    @SuppressLint("Range")
    public int toIdFromPlayerName(String name){
        SQLiteDatabase db = getReadableDatabase();
        Cursor c = db.rawQuery( "select player_id from player where name='"+name+"';", null);
        if(c == null){
            return -1;
        }
        if(c.moveToFirst()) {
            return (c.getInt(c.getColumnIndex("player_id")));
        }
        return -1;
    }

    public boolean addPlayer(String name){
        SQLiteDatabase db = getWritableDatabase();
        boolean canInsert = !hasPlayer(name)
                // 名前が空白か判定
                && !name.trim()
                    .replaceAll("[\\h]+", "")// 全角スペースを除去
                    .equals("");
        if (canInsert) {
            Log.i("addPlayer","insert "+name);
            db.beginTransaction();
            db.execSQL("insert into player (name) values ('" + name + "');");
            db.setTransactionSuccessful();
            db.endTransaction();
        }
        return canInsert;
    }

    public boolean hasPlayer(String name){
        SQLiteDatabase db = getReadableDatabase();
        Cursor c = db.rawQuery( "select * from player where name='"+name+"';", null);
        boolean flg = c.getCount() > 0;
        c.close();
        return flg;
    }

    public boolean hasPlayer(int player_id){
        boolean flg = true;
        SQLiteDatabase db = getReadableDatabase();
        Cursor c = db.rawQuery( "select * from player where player_id="+player_id+";", null);
        if(c.getCount() == 0){
            flg = false;
        }
        c.close();
        return flg;
    }

    public PlayerItem getPlayer(int player_id){
        if (hasPlayer(player_id)){
            return playerItemQuery.select(player_id);
        }
        return null;
    }

    public PlayerItem getPlayer(String name){
        if (hasPlayer(name)){
            return playerItemQuery.select(name);
        }
        return null;
    }

    public boolean setCurrent(int player_id){
        boolean canSet = hasPlayer(player_id);
        SQLiteDatabase db;
        if(canSet){
            db = getWritableDatabase();
            db.beginTransaction();
            db.execSQL("update player set current=0;");
            db.execSQL("update player set current=1 where player_id="+player_id+";");
            db.setTransactionSuccessful();
            db.endTransaction();
        }
        return canSet;
    }

    public boolean setCurrent(String name){
        return setCurrent(toIdFromPlayerName(name));
    }

    @SuppressLint("Range")
    public PlayerItem getCurrentPlayer(){
        SQLiteDatabase db = getReadableDatabase();
        Cursor c = db.rawQuery("select * from player where current=1;", null);
        if (c.getCount() == 0)
            return null;
        else {
            int player_id;
            c.moveToFirst();
            player_id = c.getInt(c.getColumnIndex("player_id"));
            return getPlayer(player_id);
        }
    }


    public List<PlayerItem> getPlayerAll(){
        return playerItemQuery.selectAll();
    }

    //**********************************************************************************************
    // SQLiteOpenHelper関係
    //**********************************************************************************************


    public DBHelper(@Nullable Context context,
                    @Nullable String name,
                    @Nullable SQLiteDatabase.CursorFactory factory,
                    int version) {
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

