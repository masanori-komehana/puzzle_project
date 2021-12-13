#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from contextlib import closing
import sqlite3

# 接続先となるDBの名前。'/home/user/database.db'といった表現方法も可能。
dbname = 'pazzle15.db'

# コネクタ作成。dbnameの名前を持つDBへ接続する。

with closing(sqlite3.connect(dbname)) as connection:

    c = connection.cursor()
    
    players = [("Alice",),("Bob",)]
    print(players)
    c.executemany("insert into player(name) values (?);", players)
    connection.commit()

