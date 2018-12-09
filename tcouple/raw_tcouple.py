#!/usr/bin/env python

import serial
import time

from datetime import datetime

ser = serial.Serial(
    port='/dev/ttyACM0',\
    baudrate=9600,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
        timeout=1)

print("connected to: " + ser.portstr)

line = []

while True:
    for c in ser.read():
        #print("Got: {}".format(int(c)))
        if int(c) == 10:
            lineStr = ''.join(line)
            print(lineStr)
            line = []
        else:
            line.append(chr(c))

ser.close()

