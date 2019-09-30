
https://github.com/arendst/Sonoff-Tasmota

Erase flash memory

esptool -cp COM5 -cb 115200 -ce -v

Upload firmware

Once the erase is complete, put device back into programming mode and upload the firmware

esptool -cp /dev/ttyACM0 -cb 115200 -bm dout -cf sonoff.bin -v
