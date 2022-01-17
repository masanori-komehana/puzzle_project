package com.fuchu.st202109.kadai05;

import android.annotation.SuppressLint;
import android.content.Context;
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

public class ResultItem {

    public static class Query {
        DBHelper dbHelper;

        public Query(DBHelper dbHelper) {
            this.dbHelper = dbHelper;
        }

        @SuppressLint("Range")
        public ResultItem select(int id){
            SQLiteDatabase db = dbHelper.getReadableDatabase();
            ResultItem resultItem = null;
            Cursor c = db.rawQuery( "select * from "+TABLE_NAME+" where id="+id+";", null);
            if(c == null){
                return null;
            }
            if(c.moveToFirst())
                resultItem = new ResultItem(
                        c.getInt(c.getColumnIndex("id")),
                        c.getInt(c.getColumnIndex("player_id")),
                        c.getLong(c.getColumnIndex("pazzletime")),
                        c.getInt(c.getColumnIndex("movecount")),
                        c.getString(c.getColumnIndex("playdatetime"))
                );
            c.close();
            return resultItem;
        }




        @SuppressLint("Range")
        private List<ResultItem> getItems(String name, Cursor c){
            List<ResultItem> lst = new ArrayList<>();
            if (c == null) return lst;
            if(c.moveToFirst()){
                do {
                    lst.add(new ResultItem(
                            c.getInt(c.getColumnIndex("id")),
                            c.getInt(c.getColumnIndex("player_id")),
                            name,
                            c.getLong(c.getColumnIndex("pazzletime")),
                            c.getInt(c.getColumnIndex("movecount")),
                            c.getString(c.getColumnIndex("playdatetime"))
                    ));
                }while (c.moveToNext());
            }
            c.close();

            return lst;
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
                            c.getLong(c.getColumnIndex("pazzletime")),
                            c.getInt(c.getColumnIndex("movecount")),
                            c.getString(c.getColumnIndex("playdatetime"))
                    ));
                }while (c.moveToNext());
            }
            c.close();

            return lst;
        }


        @SuppressLint("Range")
        private List<ResultItem> getItems_join(Cursor c){
            List<ResultItem> lst = new ArrayList<>();
            if (c == null) return lst;
            if(c.moveToFirst()){
                do {
                    lst.add(new ResultItem(
                            c.getInt(c.getColumnIndex("id")),
                            c.getInt(c.getColumnIndex("player_id")),
                            c.getString(c.getColumnIndex("name")),
                            c.getLong(c.getColumnIndex("pazzletime")),
                            c.getInt(c.getColumnIndex("movecount")),
                            c.getString(c.getColumnIndex("playdatetime"))
                    ));
                }while (c.moveToNext());
            }
            c.close();

            return lst;
        }

        public List<ResultItem> selectAll_join(){
            SQLiteDatabase db = dbHelper.getReadableDatabase();
            String query = "select " +
                    "result.id as 'id'," +
                    "result.player_id as 'player_id'," +
                    "player.name as 'name'," +
                    "result.pazzletime as 'pazzletime'," +
                    "result.movecount as 'movecount'," +
                    "result.playdatetime as 'playdatetime'," +
                    "result.resigned as 'resigned' " +
                    "from result inner join player" +
                    " on result.player_id = player.player_id" +
                    ";";
            Cursor c = db.rawQuery(query, null);
            return getItems_join(c);
        }

        public List<ResultItem> selectAll(){
            SQLiteDatabase db = dbHelper.getReadableDatabase();
            String query = "select * from "+TABLE_NAME+";";
            Cursor c = db.rawQuery(query, null);
            return getItems(c);
        }

        public List<ResultItem> select(PlayerItem player) {
            SQLiteDatabase db = dbHelper.getReadableDatabase();
            String query = "select * from "+TABLE_NAME+" where player_id="+player.getPlayerId()+";";
            Cursor c = db.rawQuery(query, null);
            return getItems(player.getName(), c);
        }

        public boolean add(int player_id, int pazzletime, int movecount, int resigned){
            SQLiteDatabase db = dbHelper.getWritableDatabase();
            boolean flg = true;
            db.beginTransaction();
            try {
                String query = "insert into result(player_id, pazzletime, movecount, resigned) values " +
                        "("+player_id +", "+ pazzletime +", "+ movecount +", "+ resigned + ")" +
                        ";";
                db.execSQL(query);
                db.setTransactionSuccessful();
                Log.i("ResultItem", "Add <Result id="+player_id+", " +
                        "time="+pazzletime+", " +
                        "movecount="+movecount+", " +
                        "resigned="+resigned+"> ");
            }catch (SQLException e){
                e.printStackTrace();
                flg = false;
            }finally {
                db.endTransaction();
            }
            return flg;
        }

        public List<ResultItem> getSortedItems(SortType sortType){
            SQLiteDatabase db = dbHelper.getReadableDatabase();
            Cursor c = db.rawQuery(sortType.getQuery(), null);
            return getItems_join(c);
        }

        @SuppressLint("Range")
        public ResultItem getLatest() {
            String query = "select * " +
                    "from" +
                    "    result " +
                    "order by" +
                    "    id desc " +
                    "limit 1" +
                    ";";
            SQLiteDatabase db = dbHelper.getReadableDatabase();
            ResultItem resultItem = null;
            Cursor c = db.rawQuery( query, null);
            if(c == null){
                return null;
            }
            if(c.moveToFirst())
                resultItem = new ResultItem(
                        c.getInt(c.getColumnIndex("id")),
                        c.getInt(c.getColumnIndex("player_id")),
                        c.getLong(c.getColumnIndex("pazzletime")),
                        c.getInt(c.getColumnIndex("movecount")),
                        c.getString(c.getColumnIndex("playdatetime"))
                );
            c.close();
            return resultItem;
        }




        public enum SortType {
            Latest{
                @Override
                public String getQuery() {
                    return "select" +
                            "   result.id as 'id'," +
                            "   result.player_id as 'player_id'," +
                            "   player.name as 'name'," +
                            "   result.pazzletime as 'pazzletime'," +
                            "   result.movecount as 'movecount'," +
                            "   result.playdatetime as 'playdatetime', " +
                            "   result.resigned as 'resigned' " +
                            "from" +
                            "    result inner join player " +
                            "on" +
                            "    result.player_id=player.player_id " +
                            "order by" +
                            "    result.id desc " +
                            "limit 100" +
                            ";";
                }
            },
            Moves{
                @Override
                public String getQuery() {
                    return "select" +
                            "   result.id as 'id'," +
                            "   result.player_id as 'player_id'," +
                            "   player.name as 'name'," +
                            "   result.pazzletime as 'pazzletime'," +
                            "   result.movecount as 'movecount'," +
                            "   result.playdatetime as 'playdatetime', " +
                            "   result.resigned as 'resigned' " +
                            "from" +
                            "    result inner join player " +
                            "on" +
                            "    result.player_id=player.player_id " +
                            " where result.resigned==0 " +
                            "order by" +
                            "   result.movecount asc " +
                            "limit 100" +
                            ";";
                }
            },
            Pazzletime{
                @Override
                public String getQuery() {
                    return "select" +
                            "   result.id as 'id'," +
                            "   result.player_id as 'player_id'," +
                            "   player.name as 'name'," +
                            "   result.pazzletime as 'pazzletime'," +
                            "   result.movecount as 'movecount'," +
                            "   result.playdatetime as 'playdatetime', " +
                            "   result.resigned as 'resigned' " +
                            "from" +
                            "    result inner join player " +
                            "on" +
                            "    result.player_id=player.player_id " +
                            " where result.resigned==0 " +
                            "order by" +
                            "   result.pazzletime asc " +
                            "limit 100" +
                            ";";
                }
            },
            ;
            public abstract String getQuery();
        }
    }

    public static final String TABLE_NAME = "result";
    protected int id;
    protected int player_id;
    protected String playerName;
    protected long pazzletime;
    protected int movecount;
    protected int resigned;
    protected String playdatetime;

    public ResultItem(int id, int player_id, long pazzletime, int movecount, String playdatetime) {
        this.id = id;
        this.player_id = player_id;
        this.pazzletime = pazzletime;
        this.movecount = movecount;
        this.playdatetime = playdatetime;
    }
    public ResultItem(int id, int player_id, String playerName, long pazzletime, int movecount, String playdatetime) {
        this.id = id;
        this.player_id = player_id;
        this.playerName = playerName;
        this.pazzletime = pazzletime;
        this.movecount = movecount;
        this.playdatetime = playdatetime;
    }
    public ResultItem(int id, int player_id, String playerName, long pazzletime, int movecount, String playdatetime, int resigned) {
        this.id = id;
        this.player_id = player_id;
        this.playerName = playerName;
        this.pazzletime = pazzletime;
        this.movecount = movecount;
        this.playdatetime = playdatetime;
        this.resigned = resigned;
    }

    public int getId() {
        return id;
    }

    public int getPlayerId() {
        return player_id;
    }

    public long getPazzletime() {
        return pazzletime;
    }

    public String getPazzleTimeStr(){
        long t = getPazzletime();
        return String.format(Locale.JAPAN,
                "%02d:%02d.%02d",
                t/6000,
                (t/100)%60,
                t%100);
    }

    public int getMovecount() {
        return movecount;
    }

    public String getPlaydatetime() {
        return playdatetime;
    }

    public boolean isResigned(){
        return this.resigned == 1;
    }

    @SuppressLint("SimpleDateFormat")
    public String getPlaydatetimelocal(){
        String format = "yyyy-MM-dd HH:mm:ss";
        SimpleDateFormat df1 = new SimpleDateFormat(format);
        SimpleDateFormat df2 = new SimpleDateFormat(format);
        String local;
        try {
            df1.setTimeZone(TimeZone.getTimeZone("UTC"));
            Date date = df1.parse(getPlaydatetime());
            df2.setTimeZone(TimeZone.getDefault());
            assert date != null;
            local= df2.format(date);
        } catch (ParseException | AssertionError e) {
            e.printStackTrace();
            return getPlaydatetime();
        }
        return local;
    }


    public String getPlayerName(){
        if (playerName == null){
            // TODO とってくる処理
            return "不明";
        }
        return playerName;
    }
    public String getPlayerName(Context c){
        if (playerName == null){
            DBHelper dbHelper = new DBHelper(c, null, null, 1);
            PlayerItem p = dbHelper.getPlayer(this.player_id);
            playerName = p.getName();
        }
        return playerName;
    }
}
