-- result table
drop table if EXISTS result;
create table result(
    id integer PRIMARY KEY AUTOINCREMENT, 
    player_id integer NOT NULL,
    pazzletime integer NOT NULL,
    movecount integer NOT NULL, 
    playdatetime text DEFAULT CURRENT_TIMESTAMP,
    resigned integer DEFAULT 0
);

-- player table
drop table if EXISTS player;
create table player(
    player_id integer PRIMARY KEY AUTOINCREMENT, 
    name text NOT NULL,
    created_dt text DEFAULT CURRENT_TIMESTAMP,
    current integer DEFAULT 0
);

-- プレイヤー登録
insert into player(name) values ('Alice'), ('Bob');


-- 結果入れる
insert into result(player_id, pazzletime, movecount)
VALUES 
    ((select player_id from player where name='Alice'), 9055, 85),
    ((select player_id from player where name='Alice'), 13023, 158),
    ((select player_id from player where name='Alice'), 10823, 78),
    ((select player_id from player where name='Alice'), 11544, 128),
    ((select player_id from player where name='Alice'), 33497, -1), -- 諦めた
    ((select player_id from player where name='Alice'), 13875, -1), 
    ((select player_id from player where name='Alice'), 5062, 64)
;
insert into result(player_id, pazzletime, movecount)
VALUES 
    ((select player_id from player where name='Bob'), 11035, 65),
    ((select player_id from player where name='Bob'), 14412, 132),
    ((select player_id from player where name='Bob'), 8823, 87),
    ((select player_id from player where name='Bob'), 7644, 141),
    ((select player_id from player where name='Bob'), 21472, -1), -- 諦めた
    ((select player_id from player where name='Bob'), 5448, 103)
;

insert into result(player_id, pazzletime, movecount)
VALUES 
    ((select player_id from player where name='Alice'), 600, 85),
    ((select player_id from player where name='Alice'), 600, 85),
    ((select player_id from player where name='Alice'), 1200, 85);

insert into result(player_id, pazzletime, movecount)
VALUES 
    ((select player_id from player where name='Alice'), 360900, 85);

insert into result(player_id, pazzletime, movecount)
VALUES 
    ((select player_id from player where name='Alice'), 24000, 85);

-- 全部見る
select
    result.id,
    player.name,
    result.pazzletime,
    result.movecount,
    datetime(result.playdatetime, '+9 hours') as 'playdatetime'
from
    result inner join player
on
    result.player_id=player.player_id
;

--- 時間順
select
    result.id,
    player.name,
    result.pazzletime,
    result.movecount,
    datetime(result.playdatetime, '+9 hours') as 'playdatetime'
from
    result inner join player
on
    result.player_id=player.player_id
order by
    result.pazzletime asc
;

--- 手数順
select
    result.id,
    player.name,
    result.pazzletime,
    result.movecount,
    datetime(result.playdatetime, '+9 hours') as 'playdatetime'
from
    result 
inner join 
    player
on
    result.player_id=player.player_id
order by
    result.movecount asc
;

-- プレイヤー別 統計 総プレイ回数 総プレイ時間 ベストタイム ワーストタイム 平均タイム 手数最小最多平均
select
    player.player_id,
    player.name,
    count(*) as 'play_count',
    (sum(result.pazzletime)) as 'played_time',
    (min(result.pazzletime)) as 'best_time',
    (max(result.pazzletime)) as 'worst_time',
    (avg(result.pazzletime)) as 'average_time',
    (-- 諦めたときは-1が入ってしまうのでそれを除外したい
        select min(movecount) from result
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
;



select
  strftime('%f', 80);

--

