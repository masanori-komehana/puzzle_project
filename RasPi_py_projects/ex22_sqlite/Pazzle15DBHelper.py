#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import datetime
from contextlib import closing
import sqlite3

import sqlalchemy
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.sql import func
from sqlalchemy import Column, Integer, String, DateTime
from sqlalchemy import desc, asc
from sqlalchemy.orm import sessionmaker, scoped_session

# 接続先となるDBの名前。'/home/user/database.db'といった表現方法も可能。
dbname = 'pazzle15.db'

# コネクタ作成。dbnameの名前を持つDBへ接続する。

#with closing(sqlite3.connect(dbname)) as connection:
#
#    c = connection.cursor()
#    
#    players = [("Alice",),("Bob",)]
#    print(players)
#    c.executemany("insert into player(name) values (?);", players)
#    connection.commit()

Base = declarative_base()
engine = sqlalchemy.create_engine(f"sqlite:///{dbname}", echo=True)


class Result(Base):
    __tablename__ = "result"
    id = Column(Integer, primary_key=True, autoincrement=True)
    player_id = Column(Integer, nullable=False)
    pazzletime = Column(Integer, nullable=False)
    movecount = Column(Integer, nullable=False)
    playdatetime = Column(DateTime(timezone=True), default=func.now())
    resigned = Column(Integer, server_default="0")

class Player(Base):
    __tablename__ = "player"
    player_id = Column(Integer, primary_key=True, autoincrement=True)
    name = Column(String, nullable=False)
    created_dt = Column(DateTime(timezone=True), default=func.now())
    current = Column(Integer, server_default="0")


Base.metadata.create_all(bind=engine)
SessionMaker = sessionmaker(bind=engine)
db = scoped_session(SessionMaker)


def utc_to_jst(timestamp_utc):
    datetime_utc = datetime.datetime.strptime(str(timestamp_utc) + "+0000", "%Y-%m-%d %H:%M:%S%z")
    datetime_jst = datetime_utc.astimezone(datetime.timezone(datetime.timedelta(hours=+9)))
    timestamp_jst = datetime.datetime.strftime(datetime_jst, '%Y-%m-%d %H:%M:%S')
    return timestamp_jst

def insert_player(name):
    p = Player()
    p.name = name

    db.add(instance=p)
    db.commit()

def __insert_result(player_id, moves, pzt):
    _r = Result()
    _r.player_id = player_id
    _r.moves = moves
    _r.pazzletime = pzt
    db.add(instance=_r)
    db.commit()


def add_result(name, moves, pzt):
    _q = db.query(Player.player_id)\
            .filter(Player.name == name)\
            .all()
    if len(_q) == 0:
        insert_player(name)
        return add_result(name, moves, pzt)
    elif len(_q) == 1:
        player_id = _q[0].player_id
        __insert_result(player_id, moves, pzt)
        return True, "Success!!"
    else:
        return False, "Multiple same names."

def show_statistics():
    query = """select
    player.player_id,
    player.name,
    count(*) as 'play_count',
    (sum(result.pazzletime)) as 'played_time',
    (min(result.pazzletime)) as 'best_time',
    (max(result.pazzletime)) as 'worst_time',
    (avg(result.pazzletime)) as 'average_time',
    (select min(movecount) from result
	where player_id=player.player_id and movecount!=-1
    ) as 'best_movecount',
    max(result.movecount) as 'worst_movecount',
    avg(result.movecount) as 'avg_movecount'
from
    result 
inner join 
    player
on
    result.player_id=player.player_id
group by
    player.name
;"""
    _q = db.execute(query).all()
    print(_q)
