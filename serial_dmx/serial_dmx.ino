/* This program allows you to set DMX channels over the serial port.
**
** After uploading to Arduino, switch to Serial Monitor and set the baud rate
** to 9600. You can then set DMX channels using these commands:
**
** <number>c : Select DMX channel
** <number>v : Set DMX channel to new value
**
** These can be combined. For example:
** 100c355w : Set channel 100 to value 255.
**
** For more details, and compatible Processing sketch,
** visit http://code.google.com/p/tinkerit/wiki/SerialToDmx
**
** Help and support: http://groups.google.com/group/DmxMaster */

#include <DmxMaster.h>

void setup() {
  Serial.begin(9600);
  Serial.println("SerialToDmx ready");
  Serial.println();
  Serial.println("Syntax:");
  Serial.println(" 123c : use DMX channel 123");
  Serial.println(" 45w : set current channel to value 45");

  pinMode ( 2, OUTPUT );
}

int value = 0;
int channel;

void loop() {
  int c;
  
  digitalWrite ( 2, HIGH );

  while(!Serial.available());
  c = Serial.read();
  if ((c>='0') && (c<='9')) {
    value = 10*value + c - '0';
  } else {
    if (c=='c') channel = value;
    else if (c=='w') {
      DmxMaster.write(channel, value);
      Serial.print("Set ");
      Serial.print(channel);
      Serial.print(" to ");
      Serial.print(value);
      Serial.println(" ...");
    }
    value = 0;
  }
}
