#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import csv
import time
import threading
import queue as Queue
from random import choice
from pprint import pprint

import serial

from Pazzle15DBHelper import *

class SerCom:
    def __init__(self, tty, baud='115200'):
        self.ser = serial.Serial(tty, baud, timeout=0.1)
        self.queue = Queue.Queue()

        self.event = threading.Event()
        self.thread_r = threading.Thread(target=self.recv_)
        self.thread_r.start()

        self.thread_q = threading.Thread(target=self.get_q)
        self.thread_q.start()

    def get_q(self):
        flg_board = False
        board_len = 0
        self.board = []
        b = 0
        while True:
            while self.queue.empty():
                if self.event.is_set():
                    return
            line = self.queue.get()
            lst = line.decode('utf-8').split(',')
            lst.pop()
            if flg_board:
                self.board[b] = lst
                b += 1
                if b == board_len:
                    flg_board = False
                    pprint(self.board)
            elif lst[0] == '-2':
                print('board data:')
                self.board = [None for i in range(int(lst[1]))]
                flg_board = True
                board_len = int(lst[1])
                b = 0
            else:
                movecnt  = int(lst[0])
                ms10 = int(lst[1])
                resigned = int(lst[2])
                name = "Guest"
                try:
                    with create_session() as db:
                        p = get_current_player(db)
                        name = p.name
                        ret = Result(
                            player_id=p.player_id,
                            pazzletime=ms10,
                            movecount=movecnt,
                            resigned=resigned
                        )
                        db = scoped_session(SessionMaker)    
                        db.add(ret)
                except Exeption as e:
                    print(e)
                print('pazzle record')
                print(f"move count: {movecnt}")
                print(f"time: {ms10_to_time(ms10)}")
                with open('records.csv', 'a') as f:
                    record = [name, lst[0], lst[1], lst[2]]
                    w = csv.writer(f, lineterminator='\n')
                    w.writerow(record)
                
            self.queue.task_done()

    def recv_(self):
        while not self.event.is_set():
            line = self.ser.readline()
            if len(line) > 0:
                #print(line)
                #print((line).decode('utf-8').split(','))
                self.queue.put(line)

    def send(self, data):
        # print('write', data)
        self.ser.write(data)

    def stop(self):
        self.event.set()
        self.thread_r.join()
        self.ser.close()

def ms10_to_time(ms10):
    return f"{ms10 // 6000}:{(ms10//100) % 60}.{ms10%100}"

def loop():
    try :
        while True:
            time.sleep(0.1)
    except KeyboardInterrupt as e:
        print('\033[0G'+"KeyboardInterrupt")
    finally:
        print("stop serial")
        ser.stop()


argv = sys.argv 
if len(argv) < 2:
    ser = SerCom('/dev/ttyACM0', '115200')
    loop()
else:
    ser = SerCom(argv[1], '115200')
    loop()

