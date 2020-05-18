#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// Configuration
const char * ssid        = "amaroq";
const char * password    = "1er4idnfu345os3o283";

#define M_IN1 0
#define M_IN2 2
#define M_IN3 14
#define M_IN4 12

unsigned int lastTime;
unsigned int currTime;

// Initialize
void setup() {

   // Start and connect to WIFI
   WiFi.mode(WIFI_STA);
   WiFi.setSleepMode(WIFI_NONE_SLEEP);
   WiFi.begin(ssid, password);

   while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      delay(5000);
      ESP.restart();
   }
   delay(1000);

   // Start logging
   delay(1000);

   // Firmware download start callback
   ArduinoOTA.onStart([]() {
      if (ArduinoOTA.getCommand() == U_FLASH) { }
      else { }
   });

   // Firmware download done callback
   ArduinoOTA.onEnd([]() { });

   // Firmware download progress
   ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      //logPrintf("Download Progress: %u%%",(progress / (total / 100)))
   });

   ArduinoOTA.onError([](ota_error_t error) {
      //if      (error == OTA_AUTH_ERROR)    {logPrintf("Download Failed: Auth Failed")}
      //else if (error == OTA_BEGIN_ERROR)   {logPrintf("Download Failed: Begin Failed")}
      //else if (error == OTA_CONNECT_ERROR) {logPrintf("Download Failed: Connect Failed")}
      //else if (error == OTA_RECEIVE_ERROR) {logPrintf("Download Failed: Receive Failed")}
      //else if (error == OTA_END_ERROR)     {logPrintf("Download Failed: End Failed")}
   });

   ArduinoOTA.begin();

   pinMode(M_IN1,OUTPUT);
   pinMode(M_IN2,OUTPUT);
   pinMode(M_IN3,OUTPUT);
   pinMode(M_IN4,OUTPUT);

   digitalWrite(M_IN1,LOW);
   digitalWrite(M_IN2,LOW);
   digitalWrite(M_IN3,LOW);
   digitalWrite(M_IN4,LOW);

   lastTime = millis();
}


// Service Loop
void loop() {
   ArduinoOTA.handle();

   currTime = millis();

   if ( (currTime - lastTime) < 5000 ) {
      digitalWrite(M_IN1,HIGH);
      digitalWrite(M_IN2,LOW);
      digitalWrite(M_IN3,LOW);
      digitalWrite(M_IN4,LOW);
   }
   else if ( (currTime - lastTime) < 10000 ) {
      digitalWrite(M_IN1,LOW);
      digitalWrite(M_IN2,LOW);
      digitalWrite(M_IN3,HIGH);
      digitalWrite(M_IN4,LOW);
   }
   else if ( (currTime - lastTime) < 15000 ) {
      digitalWrite(M_IN1,LOW);
      digitalWrite(M_IN2,HIGH);
      digitalWrite(M_IN3,LOW);
      digitalWrite(M_IN4,LOW);
   }
   else if ( (currTime - lastTime) < 20000 ) {
      digitalWrite(M_IN1,LOW);
      digitalWrite(M_IN2,LOW);
      digitalWrite(M_IN3,LOW);
      digitalWrite(M_IN4,HIGH);
   }
   else lastTime = currTime;
}

