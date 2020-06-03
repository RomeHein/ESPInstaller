#include <WiFi.h>
#include <ESPmDNS.h>
#include "ServerHandler.h"

//unmark following line to enable debug mode
#define __debug

const char* ssid       = "RPiHotspotN";
const char* password   = "jdtswke3m@W";
const char* dns = "esp-installer";
ServerHandler *serverhandler;

bool tried = false;

void setup(void) {
  Serial.begin(115200);
  #ifdef __debug
    Serial.println(F("[SETUP] Connecting to wifi"));
  #endif
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      #ifdef __debug
        Serial.print(".");
      #endif
  }
  #ifdef __debug
    Serial.println(F("[SETUP] CONNECTED"));
  #endif

  if (MDNS.begin(dns)) {
    #ifdef __debug
      Serial.printf("[SETUP] Web interface available on: %s or ",dns);
    #endif
  }
  Serial.println(WiFi.localIP());

  // Set server handler.
  serverhandler = new ServerHandler();
  serverhandler->begin();

}

void loop(void) {
  if ( WiFi.status() ==  WL_CONNECTED ) {
    serverhandler->server.handleClient();
    if (!tried) {
      tried = true;
      serverhandler->handleRepoList();
    }
  } else {
    // wifi down, try reconnecting, otherwise restart ESP
    WiFi.begin();
    int count = 0;
    while (WiFi.status() != WL_CONNECTED && count < 200 ) 
    {
      delay(500);
      ++count;
      #ifdef __debug
        Serial.printf("[MAIN_LOOP] Wifi deconnected: attempt %i\n", count);
      #endif
      if (count == 200) {
        Serial.println(F("[MAIN_LOOP] Failed to reconnect, restarting now."));
        ESP.restart();
      } 
    }
  }
}
