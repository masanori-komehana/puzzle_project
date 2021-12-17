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

import com.fuchu.st202109.kadai05.ResultItem.Query.SortType;

import java.util.List;

public class RankingActivity extends AppCompatActivity {

    private DBHelper dbHelper = new DBHelper(this, null, null, 1);

    private ListView listView;
    protected ResultsBaseAdapter adapter;
    private List<ResultItem> items;

    private SortType currentSortType = SortType.Latest;

    enum TRankTypeButtonsInfo{
        Latest(R.id.btn_show_latest, SortType.Latest),
        Moves(R.id.btn_rank_moves, SortType.Moves),
        Pazzletime(R.id.btn_rank_time, SortType.Pazzletime);
        ;


        private final int buttonId;
        private final SortType sortType;

        TRankTypeButtonsInfo(int buttonId, SortType sortType) {
            this.buttonId = buttonId;
            this.sortType = sortType;
        }

        public int getButtonId() {
            return buttonId;
        }

        public SortType getSortType() {
            return sortType;
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ranking);

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        RankingActivity act = this;

        for(TRankTypeButtonsInfo bti : TRankTypeButtonsInfo.values()){
            Button btn = findViewById(bti.getButtonId());
            btn.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    currentSortType = bti.sortType;
                    loadResults();
                }
            });
        }



        adapter = new ResultsBaseAdapter(this, items);
        listView = (ListView)findViewById(R.id.listView);
        loadResults();
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

    public void loadResults(){
        items = dbHelper.resultItemQuery.getSortedItems(currentSortType);
        adapter.setItems(items);
        listView.setAdapter(adapter);
        adapter.notifyDataSetChanged();
        Log.i("loadResults", "Sort type: "+ currentSortType.name());
    }

    class ResultsBaseAdapter extends BaseAdapter {
        private Context context;
        private List<ResultItem> items;

        private class ViewHolder{
            TextView textTitleId;
            TextView textId;
            TextView textName;
            TextView textPlayDT;
            TextView textMoves;
            TextView textPzt;
        }

        public ResultsBaseAdapter(Context context, List<ResultItem> items) {
            this.context = context;
            this.items = items;
        }

        public void setItems(List<ResultItem> items) {
            this.items = items;
        }

        @SuppressLint("SetTextI18n")
        @Override
        public View getView(int position, View convertView, ViewGroup parent) {

            View view = convertView;
            ViewHolder holder;

            ResultItem item = items.get(position);

            if (view == null){
                LayoutInflater inflater =
                        (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                view = inflater.inflate(R.layout.row_result_listview, parent, false);
                holder = new ViewHolder();
                holder.textTitleId = view.findViewById(R.id.text_title_id);
                holder.textId = view.findViewById(R.id.text_id);
                holder.textName = view.findViewById(R.id.text_player_name);
                holder.textPlayDT = view.findViewById(R.id.text_playdatetime);
                holder.textMoves = view.findViewById(R.id.text_movecount);
                holder.textPzt = view.findViewById(R.id.text_pzt);
                view.setTag(holder);
            }else {
                holder = (ViewHolder) view.getTag();
            }
            boolean isResigned = item.isResigned();
            if(currentSortType == SortType.Latest){
                holder.textTitleId.setText(getResources().getString(R.string.label_id));
                holder.textId.setText("" + item.getId());
            }else{
                holder.textTitleId.setText("順位：");
                holder.textId.setText("" + (position+1));
            }
            holder.textName.setText(item.getPlayerName());
            holder.textPlayDT.setText(item.getPlaydatetimelocal());
            holder.textMoves.setText(""+item.getMovecount());
            holder.textPzt.setText(item.getPazzleTimeStr());

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