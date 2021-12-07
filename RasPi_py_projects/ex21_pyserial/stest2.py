#!/bin/env python3

from random import choice

import time

import serial
ser = serial.Serial('/dev/ttyACM0', '115200', timeout=0.1)


try:
    while True:
        v = choice('wasd')
        print(v)
        ser.write(str.encode(v))
        time.sleep(1*(32*3)/3200)
finally:
    ser.close()



