#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

// Configuration
const char * ssid        = "amaroq";
const char * password    = "1er4idnfu345os3o283";
const char * mqtt_server = "aliska.amaroq.net";
unsigned int locPort     = 8112;
unsigned int logPort     = 8111;
IPAddress    logAddress (172,16,20,1);

// Timers
const unsigned long MessagePeriod  = 10000; // 1 minute

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Variables
unsigned int lastMessage;
unsigned int currTime;

WiFiUDP logUdp;

// Macro for logging
#define logPrintf(...) logUdp.beginPacket(logAddress,logPort); logUdp.printf(__VA_ARGS__); logUdp.endPacket();

// Initialize
void setup() {

   lastMessage = millis();

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
   });

   ArduinoOTA.onError([](ota_error_t error) {
      if      (error == OTA_AUTH_ERROR)    {logPrintf("Download Failed: Auth Failed")}
      else if (error == OTA_BEGIN_ERROR)   {logPrintf("Download Failed: Begin Failed")}
      else if (error == OTA_CONNECT_ERROR) {logPrintf("Download Failed: Connect Failed")}
      else if (error == OTA_RECEIVE_ERROR) {logPrintf("Download Failed: Receive Failed")}
      else if (error == OTA_END_ERROR)     {logPrintf("Download Failed: End Failed")}
   });

   ArduinoOTA.begin();

}


// Service Loop
void loop() {
   ArduinoOTA.handle();

   currTime = millis();

   // Refresh digital values
   if ( (currTime - lastMessage) > MessagePeriod) {
      logPrintf("Wifi strenth = %i",WiFi.RSSI());
      lastMessage = currTime;
   }
}

