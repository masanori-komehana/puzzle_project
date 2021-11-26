
drop table if EXISTS result;

create table result(
    id integer NOT NULL PRIMARY KEY AUTOINCREMENT, 
    name text NOT NULL,
    pazzletime text NOT NULL,
    movecount integer NOT NULL, 
    playdatetime text DEFAULT CURRENT_TIMESTAMP
);

INSERT INTO result(name, pazzletime, movecount)
VALUES ('Alice', '1:30.55', 115);


select 
	id, 
    name, 
    pazzletime, 
    movecount, 
    datetime(playdatetime, '+9 hours') as 'playdatetime' 
from 
	result;