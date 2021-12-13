#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from Pazzle15DBHelper import *

dbname = 'pazzle15.db'

db.query(Player).delete()


for name in ["Alice", "Bob"]:
    
    _p = Player()
    _p.name = name
    db.add(instance=_p)

db.commit()



