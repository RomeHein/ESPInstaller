#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include "ServerHandler.h"

//unmark following line to enable debug mode
#define __debug

const char *ssid = "YOUR_SSID";
const char *password = "YOUR_PASS";
const char *dns = "esp-installer";

ServerHandler *serverhandler;
WiFiClientSecure client;
const char *repoPath = "https://raw.githubusercontent.com/RomeHein/ESPInstaller/master/list.json";

void getRepoList()
{
  HTTPClient http;
#ifdef __debug
  Serial.printf("[SERVER] Retrieving repo list from %s\n", repoPath);
#endif
  http.begin(client, repoPath);
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0)
  {
    serverhandler->repoList = http.getString();
    serverhandler->repoListState = 1;
#ifdef __debug
    Serial.printf("[SERVER] repo list retrieved: %s\n", serverhandler->repoList.c_str());
#endif
  }
  else
  {
    Serial.printf("[SERVER] Could not get repo list. Error %d: %s\n", httpResponseCode, http.errorToString(httpResponseCode).c_str());
    serverhandler->repoListState = -1;
  }
  http.end();
}

void getRepoInformations()
{
  HTTPClient http;
#ifdef __debug
  Serial.printf("[SERVER] Retrieving repo information from %s\n", serverhandler->repoPath.c_str());
#endif
  http.begin(client, serverhandler->repoPath.c_str());
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0)
  {
    serverhandler->repoInformation = http.getString();
    serverhandler->repoInformationState = 1;
#ifdef __debug
    Serial.printf("[SERVER] repo info retrieved: %s\n", serverhandler->repoInformation.c_str());
#endif
  }
  else
  {
    Serial.printf("[SERVER] Could not get repo information: %s\n", http.errorToString(httpResponseCode).c_str());
    serverhandler->repoInformationState = -1;
  }
  http.end();
}

void installFromRepo()
{
  if (!serverhandler->spiffPath.isEmpty())
  {
#ifdef __debug
    Serial.printf("[OTA] start SPIFFS download from: %s\n", serverhandler->spiffPath.c_str());
#endif
    t_httpUpdate_return spiffRet = httpUpdate.updateSpiffs(client, serverhandler->spiffPath);
    switch (spiffRet)
    {
    case HTTP_UPDATE_FAILED:
      Serial.printf("[OTA] Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
      return;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("[OTA] no updates");
      return;

    case HTTP_UPDATE_OK:
#ifdef __debug
      Serial.println("[OTA] SPIFFS updated");
#endif
      Serial.println("HTTP_UPDATE_OK");
      break;
    }
  }

  if (!serverhandler->binPath.isEmpty())
  {
#ifdef __debug
    Serial.printf("[OTA] start BIN download from: %s\n", serverhandler->binPath.c_str());
#endif

    t_httpUpdate_return binRet = httpUpdate.update(client, serverhandler->binPath);
    switch (binRet)
    {
    case HTTP_UPDATE_FAILED:
      Serial.printf("[OTA] Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
      serverhandler->repoInstallationState = -1;
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("[OTA] no updates");
      serverhandler->repoInstallationState = -1;
      break;

    case HTTP_UPDATE_OK:
#ifdef __debug
      Serial.println("[OTA] BIN updated");
#endif
      serverhandler->repoInstallationState = 1;
      ESP.restart();
      break;
    }
  }
}

void setup(void)
{
  Serial.begin(115200);
#ifdef __debug
  Serial.println(F("[SETUP] Connecting to wifi"));
#endif
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
#ifdef __debug
    Serial.print(".");
#endif
  }
#ifdef __debug
  Serial.println(F("[SETUP] CONNECTED"));
#endif

  if (MDNS.begin(dns))
  {
#ifdef __debug
    Serial.printf("[SETUP] Web interface available on: %s or ", dns);
#endif
  }
  Serial.println(WiFi.localIP());

  //See discussion: https://github.com/RomeHein/ESPInstaller/issues/2
  client.setInsecure();

  // Set server handler.
  serverhandler = new ServerHandler();
  serverhandler->begin();
}

void loop(void)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    serverhandler->server.handleClient();
    if (serverhandler->repoListState == 2)
    {
      serverhandler->repoListState = 0;
      getRepoList();
    }
    if (serverhandler->repoInformationState == 2 && !serverhandler->repoPath.isEmpty())
    {
      serverhandler->repoInformationState = 0;
      getRepoInformations();
    }
    if (serverhandler->repoInstallationState == 2)
    {
      serverhandler->repoInstallationState = 0;
      installFromRepo();
    }
  }
  else
  {
    // wifi down, try reconnecting, otherwise restart ESP
    WiFi.begin();
    int count = 0;
    while (WiFi.status() != WL_CONNECTED && count < 200)
    {
      delay(500);
      ++count;
#ifdef __debug
      Serial.printf("[MAIN_LOOP] Wifi deconnected: attempt %i\n", count);
#endif
      if (count == 200)
      {
        Serial.println(F("[MAIN_LOOP] Failed to reconnect, restarting now."));
        ESP.restart();
      }
    }
  }
}
