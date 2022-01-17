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
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.List;

public class RegisterActivity extends AppCompatActivity {

    private DBHelper dbHelper = new DBHelper(this, null, null, 1);

    private ListView listView;
    protected PlayersBaseAdapter adapter;
    private List<PlayerItem> items;

    public void addPlayer(){

        EditText register_player_name = findViewById(R.id.register_player_name);

        boolean isRegisterSuccess = dbHelper.addPlayer(register_player_name.getText().toString());
        if(isRegisterSuccess){
            Toast.makeText(getApplicationContext(), "登録しました", Toast.LENGTH_LONG).show();
        }else {
            Toast.makeText(getApplicationContext(), "登録に失敗しました", Toast.LENGTH_LONG).show();
        }
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        setTitle("登録変更");
        RegisterActivity act = this;

        Button btn_register = findViewById(R.id.btn_register);
        btn_register.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                addPlayer();
                loadPlayerAll();
            }
        });

        adapter = new PlayersBaseAdapter(this, items);
        listView = (ListView)findViewById(R.id.listView);
        loadPlayerAll();
    }

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {

        int id = item.getItemId();
        if(id == android.R.id.home){
            Intent intent = new Intent();
            setResult(RESULT_CANCELED, intent);
            finish();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    void loadPlayerAll(){
        items = dbHelper.getPlayerAll();
        adapter.setItems(items);
        listView.setAdapter(adapter);
        adapter.notifyDataSetChanged();
    }

    class PlayersBaseAdapter extends BaseAdapter{
        private Context context;
        private List<PlayerItem> items;

        private class ViewHolder{
            TextView textPlayerId;
            TextView textName;
            TextView textCreatedDT;
            Button selectButton;
        }

        public PlayersBaseAdapter(Context context, List<PlayerItem> items) {
            this.context = context;
            this.items = items;
        }

        public void setItems(List<PlayerItem> items) {
            this.items = items;
        }

        @SuppressLint("SetTextI18n")
        @Override
        public View getView(int position, View convertView, ViewGroup parent) {

            View view = convertView;
            ViewHolder holder;

            PlayerItem item = items.get(position);

            if (view == null){
                LayoutInflater inflater =
                        (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                view = inflater.inflate(R.layout.row_player_listview, parent, false);
                holder = new ViewHolder();
                holder.textPlayerId = view.findViewById(R.id.text_id);
                holder.textName = view.findViewById(R.id.text_player_name);
                holder.textCreatedDT = view.findViewById(R.id.text_playdatetime);
                holder.selectButton = view.findViewById(R.id.btn_select_player);
                view.setTag(holder);
            }else {
                holder = (ViewHolder) view.getTag();
            }
            holder.textPlayerId.setText(""+item.getPlayerId());
            holder.textName.setText(item.getName());
            Log.i("listView", "set Name: "+ item.getName());
            holder.textCreatedDT.setText(item.getCreated_dt_local());
            holder.selectButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    dbHelper.setCurrent(item.player_id);
                    loadPlayerAll();
                    Intent intent = new Intent();
                    setResult(RESULT_OK, intent);
                    finish();
                }
            });
            if (item.isCurrent()){
                holder.selectButton.setEnabled(false);
                holder.selectButton.setText("選択中");
            }


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