#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <stdint.h>

// Configuration
const char * ssid        = "amaroq";
const char * password    = "1er4idnfu345os3o283";

const float azAdj = 10.0;
const float elAdj = 0.0;

WiFiServer dbgServer(8192);
WiFiClient dbgClient;

WiFiServer rotServer(4533);
WiFiClient rotClient;

char rotBuffer[200];
uint32_t rotCount;

char locBuffer[200];
uint32_t locCount;

char ch;

char mark[20];
char val0[20];
char val1[20];

int32_t ret;

double az;
double el;

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

   ArduinoOTA.begin();
   dbgServer.begin();
   rotServer.begin();

   Serial.begin(9600);

   rotCount = 0;
   locCount = 0;
}


// Service Loop
void loop() {

   ArduinoOTA.handle();

   ///////////////////////////////////////////////
   // Debug client port data
   ///////////////////////////////////////////////
   if (!dbgClient) dbgClient = dbgServer.available();

   if (dbgClient) {
      while (dbgClient.available() > 0) {
         ch = dbgClient.read();
         Serial.write(ch);
      }
   }

   ///////////////////////////////////////////////
   // rotctl client port data
   ///////////////////////////////////////////////
   if (!rotClient) rotClient = rotServer.available();

   if (rotClient) {
      while (rotClient.available() > 0) {
         ch = rotClient.read();
         if ( rotCount > 190 ) rotCount = 0;
         rotBuffer[rotCount++] = ch;
         rotBuffer[rotCount] = 0;

         if ( ch == '\n' ) {

            ret = sscanf(rotBuffer,"%s %s %s", mark, val0, val1);

            // Position set
            if ( ret == 3 && strcmp(mark,"P") == 0 ) {
               az = atof(val0) + azAdj;
               el = atof(val1) + elAdj;

               Serial.print("AZ");
               Serial.print(az,2);
               Serial.print(" EL");
               Serial.print(el,2);
               Serial.print("\n");

               if ( rotClient ) {
                  rotClient.print("RPRT 0\n");
               }

               if ( dbgClient ) {
                  dbgClient.print("Request: ");
                  dbgClient.print(az);
                  dbgClient.print(" ");
                  dbgClient.println(el);
               }
            }

            // Position request
            else if ( ret == 1 && strcmp(mark,"p") == 0 ) {
               Serial.print("AZ EL\n");
            }

            else {
               if ( rotClient ) {
                  rotClient.print("RPRT -1\n");
               }
            }
            rotCount = 0;
            rotBuffer[0] = 0;
         }
      }
   }

   ///////////////////////////////////////////////
   // local serial data
   ///////////////////////////////////////////////
   while (Serial.available() > 0) {
      ch = Serial.read();
      if ( locCount > 190 ) locCount = 0;
      locBuffer[locCount++] = ch;
      locBuffer[locCount] = 0;

      if ( ch == '\n' ) {
         if ( strncmp("AZ", locBuffer, 2) == 0 ) {
            ret = sscanf(locBuffer,"AZ%s EL%s",val0, val1);

            // Position set
            if ( ret == 2 ) {
               az = atof(val0) - azAdj;
               el = atof(val1) - elAdj;

               rotClient.println(az);
               rotClient.println(el);
            }
         }

         else if ( dbgClient ) dbgClient.print(locBuffer);

         locCount = 0;
         locBuffer[0] = 0;
      }
   }
}

