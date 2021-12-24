package com.fuchu.st202109.kadai05;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.util.TypedValue;
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

        setTitle("ランキング");
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

    // TODO 設定できる用にする 2つにする
    protected boolean [] v1 = {
            true, true, true, true
    };
    protected boolean [] v2 = {
            false, false, true, true
    };
    protected boolean [] v3 = {
            true, true, false, false
    };
    class ResultsBaseAdapter extends BaseAdapter {
        private Context context;
        private List<ResultItem> items;

        private class ViewHolder{
            TextView textTitleId;
            TextView textId;
            TextView textName;
            TextView textPlayDT;
            TextView textMoves;
            TextView textTitleMoves;
            TextView textPzt;
            TextView textTitlePzt;

            public TextView[] getTextViews() {
                TextView [] textViews = {
                        textMoves,
                        textTitleMoves,
                        textPzt,
                        textTitlePzt,
                };
                return textViews;
            }

            public void setTextViewsSize(float size) {
                for (TextView t: getTextViews()) {
                    t.setTextSize(TypedValue.COMPLEX_UNIT_SP, size);
                }
            }
        }

        public ResultsBaseAdapter(Context context, List<ResultItem> items) {
            this.context = context;
            this.items = items;
        }

        public void setItems(List<ResultItem> items) {
            this.items = items;
        }

        private  boolean[] getVisibilities(SortType s){

            switch (s){
                case Latest:
                    return v1;
                case Pazzletime:
                    return v2;
                case Moves:
                    return v3;
            }
            return null;
        }

        private void setViewVisibilities(SortType s, ViewHolder v){
            boolean[] visibilities = getVisibilities(s);
            for (int i = 0; i < 4; i++) {
                TextView view = v.getTextViews()[i];
                if(visibilities[i]){
                    view.setVisibility(View.VISIBLE);
                }else{
                    view.setVisibility(View.GONE);
                }
            }
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
                holder.textTitleMoves = view.findViewById(R.id.text_title_movecount);
                holder.textMoves = view.findViewById(R.id.text_movecount);
                holder.textTitlePzt = view.findViewById(R.id.text_title_pzt);
                holder.textPzt = view.findViewById(R.id.text_pzt);
//                view.findViewById(R.id.text_title_player_name).setVisibility(View.GONE);
                view.setTag(holder);
            }else {
                holder = (ViewHolder) view.getTag();
            }
            boolean isResigned = item.isResigned();


            float textSize;
            switch (currentSortType){
                case Latest:
                    textSize = 20f;
                    holder.textTitleId.setText(getResources().getString(R.string.label_id));
                    holder.textTitleId.setTextSize(textSize);
                    holder.textId.setText("" + item.getId());
                    holder.textId.setTextSize(textSize);
                    holder.setTextViewsSize(textSize);
                    break;
                case Moves:
                case Pazzletime:
                    textSize = 40f;
                    holder.textTitleId.setText("順位：");
                    holder.textTitleId.setTextSize(textSize);
                    holder.textId.setText("" + (position+1));
                    holder.textId.setTextSize(textSize);
                    holder.setTextViewsSize(textSize);
                    break;
                default:
            }

            holder.textName.setText(item.getPlayerName());
            holder.textPlayDT.setText(item.getPlaydatetimelocal());
            holder.textMoves.setText(""+item.getMovecount());
            holder.textPzt.setText(item.getPazzleTimeStr());
            setViewVisibilities(currentSortType, holder);

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