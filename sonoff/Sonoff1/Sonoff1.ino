#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <Artnet.h>

// Per Unit Configuration
#define UNIT_NAME "sonoff1"
#define OUT_COUNT 1
#define DMX_BASE  400

// Outputs
const unsigned int OutputCount = OUT_COUNT;
const char * OutputCmndTopic[] = {"cmnd/" UNIT_NAME "/out1", "cmnd/" UNIT_NAME "/out2",
                                  "cmnd/" UNIT_NAME "/out3", "cmnd/" UNIT_NAME "/out4" };
const char * OutputStatTopic[] = {"stat/" UNIT_NAME "/out1", "stat/" UNIT_NAME "/out2",
                                  "stat/" UNIT_NAME "/out3", "stat/" UNIT_NAME "/out4" };
const unsigned int OutputPin[] = {12, 5, 4, 15};
const unsigned int Universe    = 1;
const unsigned int DmxAddrs[]  = {DMX_BASE + 0, DMX_BASE + 1, DMX_BASE + 2, DMX_BASE + 3};
const char * WifiTopic         = "stat/" UNIT_NAME "/wifi";

// Network Configuration
const char * ssid        = "amaroq";
const char * password    = "1er4idnfu345os3o283";
unsigned int locPort     = 8112;
unsigned int logPort     = 8111;
const char * mqtt_server = "aliska.amaroq.net";
IPAddress    logAddress (172,16,20,1);

// LED Pins
const unsigned int LedPin    = 13;
const unsigned int LedPeriod = 100;

// Timers
const unsigned long StatusPeriod = 60000; // 1 minute
const unsigned long DmxTimeout   = 60000; // 1 minute

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Variables
WiFiClient espClient;
//PubSubClient client(espClient);
WiFiUDP logUdp;
ArtnetReceiver artnet;

unsigned int lastStatus = millis();
unsigned int x;
unsigned int currTime;

unsigned long lastDmx;

unsigned int outputLevel[OutputCount];

char valueStr[50];
unsigned long ledTime;

// Macro for logging
//#define logPrintf(...) logUdp.beginPacket(logAddress,logPort); logUdp.printf(__VA_ARGS__); logUdp.endPacket();
#define logPrintf(...) ;

int wifiPct() {
   long rssi = WiFi.RSSI();

   if ( rssi >= -50 ) return 100;
   else if ( rssi <= -100 ) return 0;
   else return (int(2 * (rssi + 100)));
}


// MQTT Message Received
//void callback(char* topic, byte* payload, unsigned int length) {
//   digitalWrite(LedPin,LOW);
//   ledTime = millis();
//
//   for (x=0; x < OutputCount; x++) {
//
//      // Topic match
//      if ( strcmp(OutputCmndTopic[x],topic) == 0 ) {
//
//         // Turn On      
//         if ( length == 2 && strncmp((char *)payload,"ON",2) == 0 ) {
//            digitalWrite(OutputPin[x],HIGH);
//            client.publish(OutputStatTopic[x],"ON");
//            outputLevel[x] = 1;
//            logPrintf("Turning on relay %i",OutputPin[x])
//         }
//
//         // Turn Off
//         else if ( length == 3 && strncmp((char *)payload,"OFF",3) == 0 ) {
//            digitalWrite(OutputPin[x],LOW);
//            client.publish(OutputStatTopic[x],"OFF");
//            outputLevel[x] = 0;
//            logPrintf("Turning off relay %i",OutputPin[x])
//         }
//      }
//   }
//}


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
            //client.publish(OutputStatTopic[x],"ON");
         }
      } else {
         if ( outputLevel[x] ) {
            digitalWrite(OutputPin[x],LOW);
            outputLevel[x] = 0;
            logPrintf("Turning off relay %i",OutputPin[x])
            digitalWrite(LedPin,LOW);
            ledTime = millis();
            //client.publish(OutputStatTopic[x],"OFF");
         }
      }
   }
   lastDmx = millis();
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

   //client.setServer(mqtt_server, 1883);
   //client.setCallback(callback);

   // Init relays
   for (x=0; x < OutputCount; x++) {
      pinMode(OutputPin[x],OUTPUT);
      digitalWrite(OutputPin[x],HIGH);
      outputLevel[x] = 1;
   }

   pinMode(LedPin,OUTPUT);
   digitalWrite(LedPin,HIGH);

   artnet.begin();
   artnet.subscribe(0,dmxRx);
}

// MQTT Connect
//void reconnect() {
//
//   // Loop until we're reconnected
//   while (!client.connected()) {
//      logPrintf("Attempting MQTT connection...")
//
//      // Create a random client ID
//      String clientId = "ESP8266Client-";
//      clientId += String(random(0xffff), HEX);
//
//      // Attempt to connect
//      if (client.connect(clientId.c_str())) {
//         logPrintf("Connected to MQTT Server");
//
//         // Subscribe
//         for (x=0; x < OutputCount; x++) {
//            client.subscribe(OutputCmndTopic[x]);
//         }
//      } else {
//         logPrintf("Failed to connect to MQTT, waiting 5 seconds.")
//         delay(5000);
//      }
//   }
//}

// Service Loop
void loop() {
   ArduinoOTA.handle();

   //if (!client.connected()) reconnect();
   //client.loop();

   currTime = millis();

   // LED Off
   if ( (currTime - ledTime) > LedPeriod ) digitalWrite(LedPin,HIGH);

   if ( (currTime - lastDmx) > DmxTimeout ) {
      for (x=0; x < OutputCount; x++) {
         digitalWrite(OutputPin[x],HIGH);
         outputLevel[x] = 1;
      }
      lastDmx = currTime;
   }

   // Refresh digital values
//   if ( (currTime - lastStatus) > StatusPeriod) {
//         if (outputLevel[x] == 1 ) 
//            client.publish(OutputStatTopic[x],"ON");
//         else
//            client.publish(OutputStatTopic[x],"OFF");
//         delay(10);
//      sprintf(valueStr,"%i",wifiPct());
//      logPrintf("Wifi strenth = %s",valueStr);
//      lastStatus = millis();
//   }

   artnet.parse();
}

