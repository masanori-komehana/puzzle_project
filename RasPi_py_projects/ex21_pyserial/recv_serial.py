#!/usr/bin/env python3

import sys
import time
import threading
import queue as Queue
from random import choice

import serial

class SerCom:
    def __init__(self, tty, baud='115200'):
        self.ser = serial.Serial(tty, baud, timeout=0.1)
        self.queue = Queue.Queue()

        self.event = threading.Event()
        self.thread_r = threading.Thread(target=self.recv_)
        self.thread_r.start()

    def recv_(self):
        while not self.event.is_set():
             line = self.ser.readline()
             if len(line) > 0:
                 print(line)
                 self.queue.put(line)

    def send(self, data):
        # print('write', data)
        self.ser.write(data)

    def stop(self):
        self.event.set()
        self.thread_r.join()
        self.ser.close()

argv = sys.argv 
if len(argv) < 2:
    ser = SerCom('/dev/ttyACM0', '115200')
    try :
        while True:
            time.sleep(0.1)
    finally:
        ser.stop()
else:
    ser = SerCom(argv[1], '115200')
    try :
        while True:
            time.sleep(0.1)
    finally:
        ser.stop()

