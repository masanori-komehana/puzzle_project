
drop table if EXISTS result;

create table result(
    id integer NOT NULL PRIMARY KEY AUTOINCREMENT, 
    player_id integer NOT NULL,
    pazzletime text NOT NULL,
    movecount integer NOT NULL, 
    playdatetime text DEFAULT CURRENT_TIMESTAMP
);

drop table if EXISTS player;

create table player(
    id integer NOT NULL PRIMARY KEY AUTOINCREMENT, 
    name text NOT NULL,
    playedcount integer DEFAULT 0,
    playedtime text DEFAULT '00:00:00.00');


insert into player(name)
values ('Alice'), ('Bob');

INSERT INTO result(name, pazzletime, movecount)
VALUES ((select id from player where name='Alice'), '1:30.55', 85);



select
    id,
    name,
    pazzletime,
    movecount,
    datetime(playdatetime, '+9 hours') as 'playdatetime'
from 
    result;

select
    result.id,
    player.name,
    result.pazzletime,
    result.movecount,
    datetime(result.playdatetime, '+9 hours') as 'playdatetime'
from
    result inner join player
on
    result.player_id=player.id
;

