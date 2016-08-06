/*
  LiquidCrystal Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 
 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>
//#include <XBee.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//XBee xbee = XBee();
//XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
//ZBRxResponse rx = ZBRxResponse();
//ModemStatusResponse msr = ModemStatusResponse();

int relay0Pin = 10;
int relay1Pin = 13;
int b0Pin     = 8;
int b1Pin     = 9;

int b0Val = 0;
int b1Val = 0;


void setup() {
  pinMode(relay0Pin,OUTPUT);
  pinMode(relay1Pin,OUTPUT);
  pinMode(b0Pin,INPUT);
  pinMode(b1Pin,INPUT);

  
  digitalWrite(b0Pin,HIGH); // Enable pullup
  digitalWrite(b1Pin,HIGH); // Enable pullup

  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
    // start serial
  //xbee.begin(9600);
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);

  b0Val = digitalRead(b0Pin);
  b1Val = digitalRead(b1Pin);

  digitalWrite(relay0Pin,!b0Val);
  digitalWrite(relay1Pin,!b1Val);

  
  //lcd.setCursor(0,0);
  //lcd.print("Test");
  
  //xbee.readPacket();
    
  //if (xbee.getResponse().isAvailable()) {
      // got something
     
    // if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        // got a zb rx packet
        
        // now fill our zb rx class
      //  xbee.getResponse().getZBRxResponse(rx);
            
        // set dataLed PWM to value of the first byte in the data
     //   lcd.setCursor(0,0);
       // lcd.print(rx.getData(0)); 
      //  lcd.print(rx.getData(1));
       // lcd.print(rx.getData(2));
       // lcd.print(rx.getData(3));
      //  lcd.print(rx.getData(4));  
      //  lcd.print(rx.getData(5)); 
      //  lcd.print(rx.getData(6));
      //  lcd.print(rx.getData(7));
      //  lcd.print(rx.getData(8));
      //  lcd.print(rx.getData(9));  
      //  lcd.print(rx.getData(10)); 
      //  lcd.print(rx.getData(11));
      //  lcd.print(rx.getData(12));
       // lcd.print(rx.getData(13));
      //  lcd.print(rx.getData(14));  
       // lcd.print(rx.getData(15));     
    // } 
//  }   
}

