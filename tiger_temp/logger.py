
import sys
sys.path.append('/amaroq/hass/pylib')

import hass_mysql
import weather_convert
import serial
import datetime

db = hass_mysql.Mysql("test")

with open("sensor_log.csv","a") as f:
    with serial.Serial('/dev/ttyACM0') as ser:

        while True:
            ln = ser.readline().decode('UTF-8')
            fields = ln.split(',')
            subFields = fields[0].split(' = ')
            temp = float(subFields[1])

            curr = weather_convert.tempCelToFar(db.getSensorCurrent('Indoor','temp'))
            tm = str(datetime.datetime.now())

            diff = temp - curr

            print(f"{tm}, {curr}, {temp}, {diff}")
            print(f"{tm}, {curr}, {temp}, {diff}",file=f)

