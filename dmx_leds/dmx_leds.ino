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

const int ledPin  = 11;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, ledPin, NEO_GRB + NEO_KHZ800);

// the setup routine runs once when you press reset:
void setup() {             
  
  // Enable DMX slave interface and start recording
  // DMX data
  dmx_slave.enable ();  
  
  // Set start address to 1, this is also the default setting
  // You can change this address at any time during the program
  dmx_slave.setStartAddress (400);
  
  strip.begin();
  strip.show();
}

void loop() {
  uint8_t x;

  // Get group a rgb
  uint8_t  ar = dmx_slave.getChannelValue(1);
  uint8_t  ag = dmx_slave.getChannelValue(2);
  uint8_t  ab = dmx_slave.getChannelValue(3);

  // Get group b rgb
  uint8_t  br = dmx_slave.getChannelValue(4);
  uint8_t  bg = dmx_slave.getChannelValue(5);
  uint8_t  bb = dmx_slave.getChannelValue(6);

  // Get group b rgb
  uint8_t  cr = dmx_slave.getChannelValue(7);
  uint8_t  cg = dmx_slave.getChannelValue(8);
  uint8_t  cb = dmx_slave.getChannelValue(9);

  // Get group b rgb
  uint8_t  dr = dmx_slave.getChannelValue(10);
  uint8_t  dg = dmx_slave.getChannelValue(11);
  uint8_t  db = dmx_slave.getChannelValue(12);

  for (x=0;  x < 15;  x++) strip.setPixelColor(x,ar,ag,ab);
  for (x=15; x < 30; x++) strip.setPixelColor(x,br,bg,bb);
  for (x=30; x < 45; x++) strip.setPixelColor(x,cr,cg,cb);
  for (x=45; x < 60; x++) strip.setPixelColor(x,dr,dg,db);

  strip.show();
  delay(100);
}

