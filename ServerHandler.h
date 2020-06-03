#pragma once

#ifndef ServerHandler_h
#define ServerHandler_h
#include <Arduino.h>
#include <WiFiClient.h>
#include <WebServer.h>

class ServerHandler {
private:
    WiFiClient client;
    void handleRoot();
    void handleNotFound();
    void handleRestart();
    void handleRepoList();
    void handleRepoInformation();
    void handleUpload();
    void install();
    void installFromRepo();
public:
    WebServer server = {80};
    void begin();
};
#endif