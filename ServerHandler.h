#pragma once

#ifndef ServerHandler_h
#define ServerHandler_h
#include <Arduino.h>
#include <WebServer.h>

class ServerHandler {
private:
    void handleRoot();
    void handleNotFound();
    void handleRestart();
    void handleRepoInformation();
    void checkRepoInformation();
    void handleRepoList();
    void checkRepoList();
    void handleUpload();
    void install();
    void handleInstallFromRepo();
public:
    WebServer server = {80};
    void begin();

    //Note: The following part is all about acting as a proxy. 
    // The esp32 will retrieved informations on our behalf and then the front side will check periodically if we have something new

    // These are flags to notify the main loop to download informations
    // State 0: no data
    // State 1: data updated
    // State 2: need update (this will trigger an http request in the main loop)
    // State -1: Error while trying to retrieve informations
    int repoListState = 0;
    int repoInformationState = 0;
    int repoInstallationState = 0;

    // http paths to be used by the main loop
    String repoPath;
    String spiffPath;
    String binPath;

    // Response saved in the main loop and checked by the server periodically
    String repoList;
    String repoInformation;

};
#endif