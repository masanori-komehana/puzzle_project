#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import datetime
from contextlib import closing, contextmanager
import sqlite3

import sqlalchemy
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.sql import func
from sqlalchemy import Column, Integer, String, DateTime
from sqlalchemy import desc, asc
from sqlalchemy.orm import sessionmaker, scoped_session

# 接続先となるDBの名前。'/home/user/database.db'といった表現方法も可能。
dbname = 'puzzle15/puzzle15.db'

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
sql_connection, echo = (f"sqlite:///{dbname}", True)
engine = sqlalchemy.create_engine(sql_connection, echo=echo)


class Result(Base):
    __tablename__ = "result"
    id = Column(Integer, primary_key=True, autoincrement=True)
    player_id = Column(Integer, nullable=False)
    pazzletime = Column(Integer, nullable=False)
    movecount = Column(Integer, nullable=False)
    playdatetime = Column(DateTime(timezone=True), default=func.now())
    resigned = Column(Integer, server_default="0")
    def __repr__(self):
        return f'<Result id={self.id} time={self.get_pzt_str()} moves={self.movecount}>'
    def get_pzt_str(self):
        pzt = int(self.pazzletime)
        return f'{pzt//6000}:{(pzt//100)%60:02}.{pzt%60:02}'
    def get_playdatetime_jst(self):
        datetime_utc = datetime.datetime.strptime(str(self.playdatetime) + "+0000", "%Y-%m-%d %H:%M:%S%z")
        datetime_jst = datetime_utc.astimezone(datetime.timezone(datetime.timedelta(hours=+9)))
        timestamp_jst = datetime.datetime.strftime(datetime_jst, '%Y-%m-%d %H:%M:%S')
        return timestamp_jst

class Player(Base):
    __tablename__ = "player"
    player_id = Column(Integer, primary_key=True, autoincrement=True)
    name = Column(String, nullable=False)
    created_dt = Column(DateTime(timezone=True), default=func.now())
    current = Column(Integer, server_default="0")
    def __repr__(self):
        return '<Player id={id} name={name!r}{current}>'.format(
                id=self.player_id, name=self.name, 
                current=(' * ' if self.current==1 else '' ))



Base.metadata.create_all(bind=engine)
SessionMaker = sessionmaker(bind=engine)

@contextmanager
def create_session():
    db = scoped_session(SessionMaker)
    try:
        yield db
        db.commit()
    except:
        db.rollback()
        raise
    finally:
        db.close()



def insert_player(db, name):
    p = Player()
    p.name = name

    db.add(instance=p)
    db.commit()

def __insert_result(db, player_id, moves, pzt):
    _r = Result()
    _r.player_id = player_id
    _r.moves = moves
    _r.pazzletime = pzt
    db.add(instance=_r)
    db.commit()


def add_result(db, name, moves, pzt):
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

def get_current_player(db):
    _q = db.query(Player)\
            .filter(Player.current == 1)\
            .first()
    return _q

def set_current_player_by_id(player_id):
    with create_session() as db:
        db.execute("update player set current=0;")
        db.commit()
        p = get_player_by_id(db, player_id)
        p.current = 1
    



def get_player_by_id_list(db, id_list):
    _q = db.query(Player)\
            .filter(Player.player_id.in_(player_id))\
            .all()
    return _q;


def get_player_by_id(db, player_id):
    _q = db.query(Player)\
            .filter(Player.player_id == player_id)\
            .first()
    return _q;



def get_current_player_id(db):
    _q = db.query(Player.player_id)\
            .filter(Player.current == 1)\
            .first()
    return _q.player_id

def all_result(db):
 
    _q = db.query(Result, Player)\
            .join(Player, Result.player_id==Player.player_id)\
            .all() 
    return _q

def show_statistics(db):
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
    return _q


