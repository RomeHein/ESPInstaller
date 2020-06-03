#pragma once

#ifndef ServerHandler_h
#define ServerHandler_h
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <WebServer.h>

class ServerHandler {
private:
    WiFiClientSecure client;
    void handleRoot();
    void handleNotFound();
    void handleRestart();
    void handleRepoInformation();
    void handleUpload();
    void install();
    void installFromRepo();
public:
    WebServer server = {80};
    void handleRepoList();
    void begin();
};
#endif