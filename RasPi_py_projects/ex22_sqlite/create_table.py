#!/usr/bin/env python3

import sqlite3

# 接続先となるDBの名前。'/home/user/database.db'といった表現方法も可能。
dbname = 'pazzle15.db'

# コネクタ作成。dbnameの名前を持つDBへ接続する。
conn = sqlite3.connect(dbname)
cur = conn.cursor()

# ここから好きなだけクエリを打つ
cur.execute("""create table result(
    id integer PRIMARY KEY AUTOINCREMENT, 
    player_id integer NOT NULL,
    pazzletime integer NOT NULL,
    movecount integer NOT NULL, 
    playdatetime text DEFAULT CURRENT_TIMESTAMP,
    resigned integer DEFAULT 0
);
""")

cur.execute("""create table player(
    player_id integer PRIMARY KEY AUTOINCREMENT, 
    name text NOT NULL,
    created_dt text DEFAULT CURRENT_TIMESTAMP,
    current integer DEFAULT 0
);
""")

# 処理をコミット
conn.commit()

# 接続を切断
conn.close()
