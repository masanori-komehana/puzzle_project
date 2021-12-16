package com.fuchu.st202109.kadai05;

import android.annotation.SuppressLint;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.TimeZone;

public class PlayerItem {

    public static class Factory {
        DBHelper dbHelper;

        public Factory(DBHelper dbHelper) {
            this.dbHelper = dbHelper;
        }

        @SuppressLint("Range")
        public PlayerItem create(int player_id){
            SQLiteDatabase db = dbHelper.getReadableDatabase();
            PlayerItem playerItem = null;
            Cursor c = db.rawQuery( "select * from "+TABLE_NAME+" where player_id="+player_id+";", null);
            if(c == null){
                return null;
            }
            if(c.moveToFirst())
                playerItem = new PlayerItem(
                        this,
                        c.getInt(c.getColumnIndex("player_id")),
                        c.getString(c.getColumnIndex("name")),
                        c.getString(c.getColumnIndex("created_dt")),
                        c.getInt(c.getColumnIndex("current"))
                );
            c.close();
            return playerItem;
        }

        @SuppressLint("Range")
        public PlayerItem create(String name){
            SQLiteDatabase db = dbHelper.getReadableDatabase();
            Cursor c = db.rawQuery( "select player_id from "+TABLE_NAME+" where name='"+name+"';", null);
            if(c == null){
                return null;
            }
            if(c.moveToFirst()) {
                int player_id = c.getInt(c.getColumnIndex("player_id"));
                return create(player_id);
            }
            return null;
        }

        @SuppressLint("Range")
        public List<PlayerItem> createAll(){
            List<PlayerItem> lst = new ArrayList<>();
            SQLiteDatabase db = dbHelper.getReadableDatabase();
            String query = "select * from player;";
            Cursor c = db.rawQuery(query, null);
            if (c == null) return lst;
            Log.i("ResultItem", "ItemCount: "+ c.getCount());
            if(c.moveToFirst()){
                do {
                    lst.add(new PlayerItem(
                            this,
                            c.getInt(c.getColumnIndex("player_id")),
                            c.getString(c.getColumnIndex("name")),
                            c.getString(c.getColumnIndex("created_dt")),
                            c.getInt(c.getColumnIndex("current"))
                    ));
                    Log.i("ResultItem", "added: "+ c.getString(c.getColumnIndex("name")));
                }while (c.moveToNext());
            }
            c.close();
            return lst;
        }
    }

    public static final String TABLE_NAME = "player";
    protected int player_id;
    protected String name;
    protected String created_dt;
    protected int current;
    protected Factory factory;



    public PlayerItem(int player_id, String name, String created_dt, int current) {
        this.factory = new Factory(new DBHelper(null,null,null,1));
        this.player_id = player_id;
        this.name = name;
        this.created_dt = created_dt;
        this.current = current;
    }
    public PlayerItem(Factory factory,int player_id, String name, String created_dt, int current) {
        this.factory = factory;
        this.player_id = player_id;
        this.name = name;
        this.created_dt = created_dt;
        this.current = current;
    }

    public boolean isCurrent(){
        return current == 1;
    }

    public int getPlayerId() {
        return player_id;
    }

    public String getName() {
        return name;
    }

    public String getCreated_dt() {
        return created_dt;
    }

    @SuppressLint("SimpleDateFormat")
    public String getCreated_dt_jst(){
        String format = "yyyy-MM-dd hh:mm:ss";
        SimpleDateFormat df1 = new SimpleDateFormat(format);
        SimpleDateFormat df2 = new SimpleDateFormat(format);
        Date date = null;
        String formattedDate = getCreated_dt();
        try {
            df1.setTimeZone(TimeZone.getTimeZone("UTC"));
            date= df1.parse(created_dt);
            df2.setTimeZone(TimeZone.getTimeZone("JST"));
            formattedDate= df2.format(date);
        } catch (ParseException e) {
            e.printStackTrace();
        }
        return formattedDate;
    }



    // TODO とってくる処理
}
