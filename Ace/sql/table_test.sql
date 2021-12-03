-- result table
drop table if EXISTS result;
create table result(
    id integer PRIMARY KEY AUTOINCREMENT, 
    player_id integer NOT NULL,
    pazzletime text NOT NULL,
    movecount text integer NOT NULL, 
    playdatetime text DEFAULT CURRENT_TIMESTAMP
);

-- player table
drop table if EXISTS player;
create table player(
    player_id integer PRIMARY KEY AUTOINCREMENT, 
    name text NOT NULL,
    created_dt text DEFAULT CURRENT_TIMESTAMP,
    current integer DEFAULT 0
);

-- �v���C���[�o�^
insert into player(name) values ('Alice'), ('Bob');


-- ���ʓ����
insert into result(player_id, pazzletime, movecount)
VALUES 
    ((select player_id from player where name='Alice'), '00:01:30.550', 85),
    ((select player_id from player where name='Alice'), '00:02:10.230', 158),
    ((select player_id from player where name='Alice'), '00:01:48.230', 78),
    ((select player_id from player where name='Alice'), '00:01:55.440', 128),
    ((select player_id from player where name='Alice'), '00:05:34.970', -1), -- ���߂�
    ((select player_id from player where name='Alice'), '00:02:18.750', -1), 
    ((select player_id from player where name='Alice'), '00:00:50.620', 64)
;
insert into result(player_id, pazzletime, movecount)
VALUES 
    ((select player_id from player where name='Bob'), '00:01:50.350', 65),
    ((select player_id from player where name='Bob'), '00:02:24.120', 132),
    ((select player_id from player where name='Bob'), '00:01:28.230', 87),
    ((select player_id from player where name='Bob'), '00:01:16.440', 141),
    ((select player_id from player where name='Bob'), '00:03:34.720', -1), -- ���߂�
    ((select player_id from player where name='Bob'), '00:00:54.480', 103)
;

insert into result(player_id, pazzletime, movecount)
VALUES 
    ((select player_id from player where name='Alice'), '00:01:00.000', 85),
    ((select player_id from player where name='Alice'), '00:01:00.000', 85),
    ((select player_id from player where name='Alice'), '00:02:00.000', 85);

insert into result(player_id, pazzletime, movecount)
VALUES 
    ((select player_id from player where name='Alice'), '10:01:30.000', 85);

insert into result(player_id, pazzletime, movecount)
VALUES 
    ((select player_id from player where name='Alice'), '00:40:00.000', 85);

-- �S������
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

--- ���ԏ�
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

--- �萔��
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

-- �v���C���[�� ���v ���v���C�� ���v���C���� �x�X�g�^�C�� ���[�X�g�^�C�� ���σ^�C�� �萔�ŏ��ő�����
select
    player.player_id,
    player.name,
    count(*) as 'play_count',
    cast(
	-- 24���Ԉȏオ�o�Ȃ��̂Ŋ���Z�Ŗ������o��
        sum(strftime('%s',result.pazzletime)-strftime('%s','2000-01-01'))/3600 
    as integer) as 'played_hour',
    strftime(
    '%M',
        datetime(sum(strftime('%s',result.pazzletime)-strftime('%s','2000-01-01'))
    ,'unixepoch')) as 'played_min',
    strftime(
    '%S',
        datetime(sum(strftime('%s',result.pazzletime)-strftime('%s','2000-01-01'))
    ,'unixepoch')) as 'played_sec',
    (
        select pazzletime from result
	where 
            player_id=player.player_id
        order by
            pazzletime asc
        limit 1
    ) as 'best_time',
    (
        select pazzletime from result
	where 
            player_id=player.player_id
        order by
            pazzletime desc
        limit 1
    ) as 'worst_time',
    strftime(
    '%M',
        datetime(
            avg(strftime('%s',result.pazzletime)-strftime('%s','2000-01-01'))
        ,'unixepoch')
    ) as 'average_min',
    (
        -- unixepoch���ƕ��ς̃~���b�P�ʂ��łȂ� ��]���������ƌv�Z�ł��Ȃ��̂ł���Ȃ��ƂɁE�E�E
        avg(strftime('%s',result.pazzletime)-strftime('%s','2000-01-01') + strftime('%f',result.pazzletime) - cast(strftime('%f',result.pazzletime) as integer))
	- (cast((avg(strftime('%s',result.pazzletime)-strftime('%s','2000-01-01') + strftime('%f',result.pazzletime) - cast(strftime('%f',result.pazzletime) as integer)))/60 as integer) * 60)
    ) as 'average_sec',
    (
        -- ���߂��Ƃ���-1�������Ă��܂��̂ł�������O������
        select min(movecount) from result
	where 
            player_id=player.player_id
            and movecount!=-1
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
  strftime('%s',pazzletime)-strftime('%s','2000-01-01') + strftime('%f',pazzletime) - cast(strftime('%f',pazzletime) as integer)
from 
  result;
 

select
  strftime('%f', 80);

--

