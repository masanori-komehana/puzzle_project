#!/bin/env python3

import serial
import time
import threading
import queue as Queue
from random import choice

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

ser = SerCom('/dev/ttyACM0', '115200')
ser.send(str.encode('\e'))
try :
    while True:
        v = choice('wasd')
        ser.send(str.encode(v))
        print(v)
        time.sleep(0.1)
finally:
    ser.stop()
