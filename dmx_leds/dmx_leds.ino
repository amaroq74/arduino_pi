/*
  DMX_Slave.ino - Example code for using the Conceptinetics DMX library
  Copyright (c) 2013 W.A. van der Meeren <danny@illogic.nl>.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <Conceptinetics.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//
// CTC-DRA-13-1 ISOLATED DMX-RDM SHIELD JUMPER INSTRUCTIONS
//
// If you are using the above mentioned shield you should 
// place the RXEN jumper towards G (Ground), This will turn
// the shield into read mode without using up an IO pin
//
// The !EN Jumper should be either placed in the G (GROUND) 
// position to enable the shield circuitry 
//   OR
// if one of the pins is selected the selected pin should be
// set to OUTPUT mode and set to LOGIC LOW in order for the 
// shield to work
//

#define DMX_SLAVE_CHANNELS 12

// Configure a DMX slave controller
DMX_Slave dmx_slave ( DMX_SLAVE_CHANNELS );

const int ledPin1  = 6;
const int ledPin2  = 7;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(300, ledPin1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(300, ledPin2, NEO_GRB + NEO_KHZ800);

// the setup routine runs once when you press reset:
void setup() {             
  
  // Enable DMX slave interface and start recording
  // DMX data
  dmx_slave.enable ();  
  
  // Set start address to 1, this is also the default setting
  // You can change this address at any time during the program
  dmx_slave.setStartAddress (400);
  
  strip1.begin();
  strip1.show();
  strip2.begin();
  strip2.show();
}

void loop() {
   uint8_t x;

   for (x=0;   x < 150;  x++) {
      strip1.setPixelColor(x,dmx_slave.getChannelValue(1)+x, 
                             dmx_slave.getChannelValue(2)+x, 
                             dmx_slave.getChannelValue(3)+x);

      strip2.setPixelColor(x,dmx_slave.getChannelValue(7)+x, 
                             dmx_slave.getChannelValue(8)+x, 
                             dmx_slave.getChannelValue(9)+x);
   }

   for (x=150;  x < 300;  x++) {
      strip1.setPixelColor(x,dmx_slave.getChannelValue(4)+x, 
                             dmx_slave.getChannelValue(5)+x, 
                             dmx_slave.getChannelValue(6)+x);

      strip2.setPixelColor(x,dmx_slave.getChannelValue(10)+x, 
                             dmx_slave.getChannelValue(11)+x, 
                             dmx_slave.getChannelValue(12)+x);
   }

   strip1.show();
   strip2.show();
   delay(100);
}


