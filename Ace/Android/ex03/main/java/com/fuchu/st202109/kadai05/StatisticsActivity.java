package com.fuchu.st202109.kadai05;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

import java.util.List;
import java.util.Locale;

public class StatisticsActivity extends AppCompatActivity {

    private DBHelper dbHelper = new DBHelper(this, null, null, 1);

    private ListView listView;
    private StatisticsBaseAdapter adapter;
    private List<StatisticsItem> items;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_statistics);

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        adapter = new StatisticsBaseAdapter(this, items);
        listView = (ListView)findViewById(R.id.listView);
        loadStatisticsItems();
    }

    private void loadStatisticsItems() {
        items = dbHelper.statisticsItemQuery.getAll();
        adapter.setItems(items);
        assert adapter!=null;
        listView.setAdapter(adapter);
        adapter.notifyDataSetChanged();
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


    class StatisticsBaseAdapter extends BaseAdapter {
        private Context context;
        private List<StatisticsItem> items;

        private class ViewHolder{
            TextView textPlayerId;
            TextView textName;
            TextView textPlayCount;
            TextView textPlayedTime;
            TextView textBestTime;
            TextView textWorstTime;
            TextView textAvgTime;
            TextView textBestMoveCount;
            TextView textWorstMoveCount;
            TextView textAvgMoveCount;
        }

        public StatisticsBaseAdapter(Context context, List<StatisticsItem> items) {
            this.context = context;
            this.items = items;
        }

        public void setItems(List<StatisticsItem> items) {
            this.items = items;
        }

        @SuppressLint("SetTextI18n")
        @Override
        public View getView(int position, View convertView, ViewGroup parent) {

            View view = convertView;
            ViewHolder holder;

            StatisticsItem item = items.get(position);

            if (view == null){
                LayoutInflater inflater =
                        (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                view = inflater.inflate(R.layout.row_statistics_listview, parent, false);
                holder = new ViewHolder();
                holder.textPlayerId = view.findViewById(R.id.text_id);
                holder.textName = view.findViewById(R.id.text_player_name);
                holder.textPlayCount = view.findViewById(R.id.text_play_count);
                holder.textPlayedTime = view.findViewById(R.id.text_played_time);
                holder.textBestTime = view.findViewById(R.id.text_best_time);
                holder.textWorstTime = view.findViewById(R.id.text_worst_time);
                holder.textAvgTime = view.findViewById(R.id.text_avg_time);
                holder.textBestMoveCount = view.findViewById(R.id.text_best_movecount);
                holder.textWorstMoveCount = view.findViewById(R.id.text_worst_movecount);
                holder.textAvgMoveCount = view.findViewById(R.id.text_avg_movecount);
                view.setTag(holder);
            }else {
                holder = (ViewHolder) view.getTag();
            }

            holder.textPlayerId.setText(""+item.getPlayer_id());
            holder.textName.setText(item.getName());
            holder.textPlayCount.setText(""+item.getPlay_count());
            holder.textPlayedTime.setText(item.getPlayed_timeStr());
            holder.textBestTime.setText(item.getBest_timeStr());
            holder.textWorstTime.setText(item.getWorst_timeStr());
            holder.textAvgTime.setText(item.getAvg_timeStr());
            holder.textBestMoveCount.setText(""+item.getBest_movecount());
            holder.textWorstMoveCount.setText(""+item.getWorst_movecount());
            holder.textAvgMoveCount.setText(String.format(Locale.JAPAN,
                    "%.2f",item.getAvg_movecount()));

            return view;
        }

        // Listの要素数を返す
        @Override
        public int getCount() {
            return items.size();
        }

        // indexやオブジェクトを返す
        @Override
        public Object getItem(int position) {
            return items.get(position);
        }

        // IDを他のindexに返す
        @Override
        public long getItemId(int position) {
            return position;
        }
    }
}