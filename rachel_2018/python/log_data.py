#!/usr/bin/env python

import serial
import time
import sys

if len(sys.argv) != 2:
    print("Usage: log_data.py file")
    exit()

file = sys.argv[1]

ser = serial.Serial(
    port='/dev/ttyACM0',\
    baudrate=115200,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
        timeout=1)

print("connected to: " + ser.portstr)
print("writing to: " + file)

line = []
temps = {}
lastTme = int(time.time())

f = open(file,"w")

last = int(time.time())

while True:
    try:
        line = ser.readline().decode('utf-8')
        data = line.strip().split()

        out = '{},{},{},{},{}'.format(time.time(),data[1],data[2],data[3],data[4])
        f.write(out + '\n')

        if ( int(time.time()) != last ):
            last = int(time.time())
            print(out)

    except Exception as msg:
        print("Got exception: " + str(msg))

f.close()
ser.close()

