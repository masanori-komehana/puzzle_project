package com.fuchu.st202109.kadai05;

import android.annotation.SuppressLint;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

public class StatisticsItem {
    public static class Query{
        DBHelper dbHelper;
        public Query(DBHelper dbHelper) {
            this.dbHelper = dbHelper;
        }

        @SuppressLint("Range")
        public List<StatisticsItem> getAll(){
            String query = "select" +
                    "    player.player_id as 'player_id'," +
                    "    player.name as 'name'," +
                    "    count(*) as 'play_count'," +
                    "    (sum(result.pazzletime)) as 'played_time'," +
                    "    (min(result.pazzletime)) as 'best_time'," +
                    "    (max(result.pazzletime)) as 'worst_time'," +
                    "    (avg(result.pazzletime)) as 'avg_time'," +
                    "    min(result.movecount) as 'best_movecount'," +
                    "    max(result.movecount) as 'worst_movecount'," +
                    "    avg(result.movecount) as 'avg_movecount' " +
                    "from" +
                    "    result " +
                    "inner join " +
                    "    player " +
                    "on" +
                    "    result.player_id=player.player_id " +
                    "group by" +
                    "    player.name " +
                    "order by " +
                    " player.player_id asc " +
                    ";";
            List<StatisticsItem> items = new ArrayList<>();
            SQLiteDatabase db = dbHelper.getReadableDatabase();
            Cursor c = db.rawQuery(query, null);
            if (c == null) return items;
            if(c.moveToFirst()){
                do {
                    items.add(new StatisticsItem(
                            c.getInt(c.getColumnIndex("player_id")),
                            c.getString(c.getColumnIndex("name")),
                            c.getInt(c.getColumnIndex("play_count")),
                            c.getLong(c.getColumnIndex("played_time")),
                            c.getLong(c.getColumnIndex("best_time")),
                            c.getLong(c.getColumnIndex("worst_time")),
                            c.getLong(c.getColumnIndex("avg_time")),
                            c.getInt(c.getColumnIndex("best_movecount")),
                            c.getInt(c.getColumnIndex("worst_movecount")),
                            c.getDouble(c.getColumnIndex("avg_movecount"))
                    ));
                }while (c.moveToNext());
            }
            c.close();
            return items;
        } 
    }
    
    private final int player_id;
    private final String name;
    private final int play_count;
    private final long played_time;
    private final long best_time;
    private final long worst_time;
    private final long avg_time;
    private final int best_movecount;
    private final int worst_movecount;
    private final double avg_movecount;

    public StatisticsItem(int player_id, String name, 
        int play_count, long played_time, long best_time, long worst_time, long avg_time, 
        int best_movecount, int worst_movecount, double avg_movecount
    ) {
        Log.i("StatisticsItem", "Loaded: "+ name);
        this.player_id = player_id;
        this.name = name;
        this.play_count = play_count;
        this.played_time = played_time;
        this.best_time = best_time;
        this.worst_time = worst_time;
        this.avg_time = avg_time;
        this.best_movecount = best_movecount;
        this.worst_movecount = worst_movecount;
        this.avg_movecount = avg_movecount;
    }

    public int getPlayer_id() {
        return player_id;
    }

    private String getTimeStr(long t){
        return String.format(Locale.JAPAN,
                "%02d:%02d.%02d",
                t/6000,
                (t/100)%60,
                t%100);
    }

    public String getName() {
        return name;
    }

    public int getPlay_count() {
        return play_count;
    }
    
    public String getPlayed_timeStr(){
        long t = getPlayed_time();
        return String.format(Locale.JAPAN,
                "%d:%02d:%02d.%02d",
                (t/360000),
                (t/6000)%60,
                (t/100)%60,
                t%100);
    }

    public long getPlayed_time() {
        return played_time;
    }

    public String getBest_timeStr() {
        return getTimeStr(best_time);
    }

    public String getWorst_timeStr() {
        return getTimeStr(worst_time);
    }

    public String getAvg_timeStr() {
        return getTimeStr(avg_time);
    }
    
    public long getBest_time() {
        return best_time;
    }

    public long getWorst_time() {
        return worst_time;
    }

    public long getAvg_time() {
        return avg_time;
    }

    public int getBest_movecount() {
        return best_movecount;
    }

    public int getWorst_movecount() {
        return worst_movecount;
    }

    public double getAvg_movecount() {
        return avg_movecount;
    }
}
