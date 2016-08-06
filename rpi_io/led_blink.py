#!/usr/bin/env python

import wiringpi2 as wiringpi

#To initialise WiringPi pin mode
#wiringpi.wiringPiSetup()
#wiringpi.wiringPiSetupSys()

#To initialise GPIO mode
wiringpi.wiringPiSetupGpio()

#To initialise Physical P1 header pin mode
#wiringpi.wiringPiSetupPhys()

print (wiringpi.piBoardRev())

#wiringpi.pinMode(port_or_pin_number, X)
#where port_number is the GPIO port (or pin) number
#X = 0 for input,
#X = 1 for output,
#X = 2 for alternative function (e.g. PWM on port 18)

# Activate pull-up
#wiringpi.pullUpDnControl(pin_or_port_num, 2)
# Activate pull-down
#wiringpi.pullUpDnControl(pin_or_port_num, 1)
# Deactivate both pull-up and pull-down
#wiringpi.pullUpDnControl(pin_or_port_num, 0)

# wiringpi numbers  
#for i in range(0,8):
    #wiringpi.pinMode(i,1)
    #wiringpi.pullUpDnControl(i,1)

wiringpi.pinMode(17,1)
wiringpi.pinMode(18,1)

#wiringpi.pinMode(6, 0) # sets WP pin 6 to input  
#wiringpi.pinMode(5, 1) # sets WP pin 5 to output  
#wiringpi.pinMode(1, 2) # sets WP pin 1 to PWM mode  

#my_input = wiringpi.digitalRead(25)

#for i in range(0,8):
#    state = wiringpi.digitalRead(i)
#    print ('Pin %d is %d' % (i,state))

while True:
    wiringpi.digitalWrite(17, 1) # sets port/pin to 0 (0V)
    wiringpi.digitalWrite(18, 1) # sets port/pin to 0 (0V)
    wiringpi.delay(1000)
    wiringpi.digitalWrite(17, 0) # sets port/pin to 0 (0V)
    wiringpi.digitalWrite(18, 0) # sets port/pin to 0 (0V)
    wiringpi.delay(1000)
    

#wiringpi.digitalWrite(port_or_pin_number, 0) # sets port/pin to 0 (0V)
#wiringpi.digitalWrite(port_or_pin_number, 1) # sets port/pin to 1 (3.3V)

#wiringpi.digitalWrite(24, 0) # sets port 24 to 0 (0V, off) – may be unnecessary, but I prefer to do it
#wiringpi.pinMode(24, 0) # sets GPIO 24 to input Mode

