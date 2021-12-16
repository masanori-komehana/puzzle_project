package com.fuchu.st202109.kadai05;

import android.annotation.SuppressLint;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;

import java.util.ArrayList;
import java.util.List;

public class ResultItem {

    public static class Factory {
        DBHelper dbHelper;

        public Factory(DBHelper dbHelper) {
            this.dbHelper = dbHelper;
        }

        @SuppressLint("Range")
        public ResultItem create(int id){
            SQLiteDatabase db = dbHelper.getReadableDatabase();
            ResultItem playerItem = null;
            Cursor c = db.rawQuery( "select * from "+TABLE_NAME+" where id="+id+";", null);
            if(c == null){
                return null;
            }
            if(c.moveToFirst())
                playerItem = new ResultItem(
                        c.getInt(c.getColumnIndex("id")),
                        c.getInt(c.getColumnIndex("player_id")),
                        c.getInt(c.getColumnIndex("pazzletime")),
                        c.getInt(c.getColumnIndex("movecount")),
                        c.getString(c.getColumnIndex("playdatetime"))
                );
            c.close();
            db.close();
            return playerItem;
        }


        @SuppressLint("Range")
        private List<ResultItem> getItems(Cursor c){
            List<ResultItem> lst = new ArrayList<>();
            if (c == null) return lst;
            if(c.moveToFirst()){
                do {
                    lst.add(new ResultItem(
                            c.getInt(c.getColumnIndex("id")),
                            c.getInt(c.getColumnIndex("player_id")),
                            c.getInt(c.getColumnIndex("pazzletime")),
                            c.getInt(c.getColumnIndex("movecount")),
                            c.getString(c.getColumnIndex("playdatetime"))
                    ));
                }while (c.moveToNext());
            }
            c.close();

            return lst;
        }

        public List<ResultItem> createAll(){
            SQLiteDatabase db = dbHelper.getReadableDatabase();
            String query = "select * from "+TABLE_NAME+";";
            Cursor c = db.rawQuery(query, null);
            return getItems(c);
        }

        public List<ResultItem> createWithPlayer(PlayerItem player) {
            SQLiteDatabase db = dbHelper.getReadableDatabase();
            String query = "select * from "+TABLE_NAME+" where player_id="+player.getPlayerId()+";";
            Cursor c = db.rawQuery(query, null);
            return getItems(c);
        }
    }

    public static final String TABLE_NAME = "result";
    protected int id;
    protected int player_id;
    protected int pazzletime;
    protected int movecount;
    protected String playdatetime;

    public ResultItem(int id, int player_id, int pazzletime, int movecount, String playdatetime) {
        this.id = id;
        this.player_id = player_id;
        this.pazzletime = pazzletime;
        this.movecount = movecount;
        this.playdatetime = playdatetime;
    }

    public int getId() {
        return id;
    }

    public int getPlayerId() {
        return player_id;
    }

    public int getPazzletime() {
        return pazzletime;
    }

    public int getMovecount() {
        return movecount;
    }

    public String getPlaydatetime() {
        return playdatetime;
    }

    // TODO とってくる処理
}
