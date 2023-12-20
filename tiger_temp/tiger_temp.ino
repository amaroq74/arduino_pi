
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11
#define OUTPWM 3

#define LOW_TEMP 50.0
#define PT1_TEMP 67.5
#define MID_TEMP 85.0
#define PT2_TEMP 102.5
#define MAX_TEMP 120.0

#define MIN_OUT 7.0
#define PT1_OUT 28.0
#define MID_OUT 48.0
#define PT2_OUT 68.0
#define MAX_OUT 105.0

#define OFFSET_TEMP -5.0

#define SLOPE  ((PT2_OUT - PT1_OUT) / (PT2_TEMP - PT1_TEMP))

DHT dht(DHTPIN, DHTTYPE);

float t;
unsigned int out;

void setup(){
  Serial.begin(9600);
  dht.begin();
  pinMode(OUTPWM,OUTPUT);
}

void loop(){
   t = dht.readTemperature(true);
   t += OFFSET_TEMP;

   out = int(((t - PT1_TEMP) * SLOPE) + PT1_OUT);
   
   analogWrite(OUTPWM, out);

   Serial.print("Temperature = ");
   Serial.print(t);
   Serial.print(", Out = ");
   Serial.println(out);
   delay(2000);
}
