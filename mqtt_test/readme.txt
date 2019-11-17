
https://github.com/arendst/Sonoff-Tasmota

Erase flash memory

esptool -cp COM5 -cb 115200 -ce -v

Upload firmware

Once the erase is complete, put device back into programming mode and upload the firmware

esptool -cp /dev/ttyACM0 -cb 115200 -bm dout -cf sonoff.bin -v

esptool.py --port COM5 erase_flash

esptool.py --port /dev/ttyACM0 write_flash -fs 1MB -fm dout 0x0 sonoff.bin
