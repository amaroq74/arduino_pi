#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <Artnet.h>

// Configuration
const char * ssid        = "amaroq";
const char * password    = "1er4idnfu345os3o283";
unsigned int locPort     = 8112;
unsigned int logPort     = 8111;
IPAddress    logAddress (172,16,20,1);

// Timers
const unsigned long StatusPeriod  = 60000; // 1 minute

// LED Pins
const unsigned int LedPin    = 13;
const unsigned int LedPeriod = 100;

// Outputs
const unsigned int OutputCount = 4;
const char * OutputCmndTopic[] = {"cmnd/test_switch2/out1", "cmnd/test_switch2/out2",
                                  "cmnd/test_switch2/out3", "cmnd/test_switch2/out4" };
const char * OutputStatTopic[] = {"stat/test_switch2/out1", "stat/test_switch2/out2",
                                  "stat/test_switch2/out3", "stat/test_switch2/out4" };
unsigned int OutputPin[]       = {12, 5, 4, 15};
unsigned int DmxTimeout = 10000;

// Artnet Settings
const unsigned int Universe   = 1;
const unsigned int DmxAddrs[] = {100, 101, 102, 103};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Variables
unsigned int lastStatus = millis();
unsigned int x;
unsigned int currTime;

unsigned int outputLevel[OutputCount];

WiFiClient espClient;
WiFiUDP logUdp;
ArtnetReceiver artnet;

char  valueStr[50];
unsigned long ledTime;
unsigned long dmxTime;

// Macro for logging
#define logPrintf(...) logUdp.beginPacket(logAddress,logPort); logUdp.printf(__VA_ARGS__); logUdp.endPacket();

int wifiPct() {
   long rssi = WiFi.RSSI();

   if ( rssi >= -50 ) return 100;
   else if ( rssi <= -100 ) return 0;
   else return (int(2 * (rssi + 100)));
}

// DMX Frame Receiver
void dmxRx(uint8_t * data, uint16_t size) {
   for ( x = 0; x < OutputCount; x++ ) {
      if ( data[DmxAddrs[x]-1] > 127 ) {
         if ( ! outputLevel[x] ) {
            digitalWrite(OutputPin[x],HIGH);
            outputLevel[x] = 1;
            logPrintf("Turning on relay %i",OutputPin[x])
            digitalWrite(LedPin,LOW);
            ledTime = millis();
         }
      } else {
         if ( outputLevel[x] ) {
            digitalWrite(OutputPin[x],LOW);
            outputLevel[x] = 0;
            logPrintf("Turning off relay %i",OutputPin[x])
            digitalWrite(LedPin,LOW);
            ledTime = millis();
         }
      }
   }
   dmxTime = millis();
}

// Initialize
void setup() {

   lastStatus = millis();

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
   logUdp.begin(locPort);
   delay(1000);
   logPrintf("System booted!")

   // Firmware download start callback
   ArduinoOTA.onStart([]() {
      if (ArduinoOTA.getCommand() == U_FLASH) {
         logPrintf("Starting sketch firmware upload")
      }
      else { // U_SPIFFS
         logPrintf("Starting filesystem firmware upload")
      }
   });

   // Firmware download done callback
   ArduinoOTA.onEnd([]() {
      logPrintf("Finished firmware upload. Rebooting....")
   });

   // Firmware download progress
   ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      logPrintf("Download Progress: %u%%",(progress / (total / 100)))
      digitalWrite(LedPin,LOW);
      ledTime = millis();
   });

   ArduinoOTA.onError([](ota_error_t error) {
      if      (error == OTA_AUTH_ERROR)    {logPrintf("Download Failed: Auth Failed")}
      else if (error == OTA_BEGIN_ERROR)   {logPrintf("Download Failed: Begin Failed")}
      else if (error == OTA_CONNECT_ERROR) {logPrintf("Download Failed: Connect Failed")}
      else if (error == OTA_RECEIVE_ERROR) {logPrintf("Download Failed: Receive Failed")}
      else if (error == OTA_END_ERROR)     {logPrintf("Download Failed: End Failed")}
   });

   ArduinoOTA.begin();

   // Init relays
   for (x=0; x < OutputCount; x++) {
      pinMode(OutputPin[x],OUTPUT);
      digitalWrite(OutputPin[x],LOW);
   }

   pinMode(LedPin,OUTPUT);
   digitalWrite(LedPin,HIGH);

   artnet.begin();
   artnet.subscribe(0,dmxRx);
}

// Service Loop
void loop() {
   ArduinoOTA.handle();

   currTime = millis();

   // LED Off
   if ( (currTime - ledTime) > LedPeriod ) digitalWrite(LedPin,HIGH);

   // Refresh digital values
   if ( (currTime - lastStatus) > StatusPeriod) {
      sprintf(valueStr,"%i",wifiPct());
      logPrintf("Wifi strenth = %s",valueStr);
      lastStatus = millis();
   }

   // DMX Timeout
   if ( (currTime - dmxTime) > DmxTimeout ) {
      for (x=0; x < OutputCount; x++) {
         if (outputLevel[x] == 1) {
            digitalWrite(OutputPin[x],LOW);
            outputLevel[x] = 0;
            logPrintf("Turning off relay %i due to timeout",OutputPin[x])
            delay(100);
         }
      }
      dmxTime = millis();
   }
   artnet.parse();
}

