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
temps = {}
lastTme = int(time.time())

f = open("temp_log.csv","a")

while True:
    for c in ser.read():
        #print("Got: {}".format(int(c)))
        if int(c) == 10:
            lineStr = ''.join(line)
            lineSplit = lineStr.split()

            if lineSplit[0] == "#1:":
                temps[0] = lineSplit[1]
            elif lineSplit[0] == "#2:":
                temps[1] = lineSplit[1]
            elif lineSplit[0] == "#3:":
                temps[2] = lineSplit[1]
            elif lineSplit[0] == "#4:":
                temps[3] = lineSplit[1]
            elif lineSplit[0] == "#5:":
                temps[4] = lineSplit[1]
            elif lineSplit[0] == "#6:":
                temps[5] = lineSplit[1]

            line = []

            currTme = int(time.time())
            if currTme != lastTme:
                lastTme = currTme
                dt = datetime.now()
                if len(temps) == 6:
                    pstr = "{}, {:%H:%M:%S}, {}, {}, {}, {}, {}, {}".format(currTme,dt,temps[0],temps[1],temps[2],temps[3],temps[4],temps[5])
                    print(pstr)
                    f.write(pstr+"\n")

        else:
            line.append(chr(c))

f.close()
ser.close()

