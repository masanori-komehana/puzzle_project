#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from Pazzle15DBHelper import *

q = db.query(Player).all()
print(len(q))
for p in q:
    print(p.player_id, p.name, utc_to_jst(p.created_dt))

