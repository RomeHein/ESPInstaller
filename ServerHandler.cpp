#include "ServerHandler.h"
#include <HTTPClient.h>
#include <Update.h>
#include <HTTPUpdate.h>
# include "index.h"

//unmark following line to enable debug mode
#define __debug

const String repoPath = "https://raw.githubusercontent.com/RomeHein/ESPInstaller/list.json";

void ServerHandler::begin()
{
    #ifdef __debug  
        Serial.println("[Server]: init");
    #endif
    server.onNotFound([this]() {handleNotFound(); });
    server.on("/",HTTP_GET, [this]() { handleRoot();});
    server.on("/repo/info",HTTP_POST, [this]() { handleRepoInformation();});
    server.on("/repo/list",HTTP_GET, [this]() { handleRepoList();});
    server.on("/update",HTTP_POST,[this]() { handleUpload(); }, [this]() { install(); });

    server.begin();
}

// Main
void ServerHandler::handleRoot()
{
    server.send(200, "text/html", MAIN_page);
}

void ServerHandler::handleNotFound()
{
    #ifdef __debug  
        Serial.println("[Server]: page not found");
    #endif
    server.send(404, "text/plain", "Not found");
}

void ServerHandler::handleRepoList()
{
    HTTPClient http;
    #ifdef __debug
        Serial.printf("[SERVER] Retrieving repo list from %s\n",repoPath.c_str());
    #endif
    http.begin(client,repoPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode>0) {
        #ifdef __debug
        Serial.println("[SERVER] repo list retrieved");
        #endif
        server.send(200, "text/json", http.getString());
    }
    else {
        Serial.printf("[SERVER] Could not get repo list: %s\n", http.errorToString(httpResponseCode).c_str());
    }
    http.end();
}

void ServerHandler::handleRepoInformation()
{
    if (server.hasArg("repo")) {
        const char* repoPath = server.arg("repo").c_str();
        HTTPClient http;
        #ifdef __debug
            Serial.printf("[SERVER] Retrieving repo information from %s\n",repoPath);
        #endif
        http.begin(client,repoPath);
        int httpResponseCode = http.GET();
        if (httpResponseCode>0) {
            #ifdef __debug
            Serial.println("[SERVER] repo info retrieved");
            #endif
            server.send(200, "text/json", http.getString());
        }
        else {
            Serial.printf("[SERVER] Could not get repo information: %s\n", http.errorToString(httpResponseCode).c_str());
        }
        http.end();
        return;
    }
    server.send(404, "text/plain", "Not found");
}

void ServerHandler::handleRestart() {
    server.send(200, "text/plain", "OK");
    ESP.restart();
}

void ServerHandler::handleUpload()
{
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart(); 
}

void ServerHandler::install()
{
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("[SERVER] Update: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
            Update.printError(Serial);
        }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
        /* flashing firmware to ESP*/
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
            Update.printError(Serial);
        }
    } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {
            Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
            Update.printError(Serial);
        }
    } 
}

void ServerHandler::installFromRepo() {
    if (server.hasArg("spiffsPath")) {
        const String spiffsPath = server.arg("spiffsPath");
        #ifdef __debug
            Serial.printf("[OTA] start SPIFFS download from: %s\n", spiffsPath.c_str());
        #endif
        t_httpUpdate_return spiffRet = httpUpdate.updateSpiffs(client, spiffsPath);
        switch (spiffRet) {
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

    if (server.hasArg("binPath")) {
        const String binPath = server.arg("binPath");
        #ifdef __debug
            Serial.printf("[OTA] start BIN download from: %s\n", binPath.c_str());
        #endif

        t_httpUpdate_return binRet = httpUpdate.update(client, binPath);
        switch (binRet) {
            case HTTP_UPDATE_FAILED:
            Serial.printf("[OTA] Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
            
            break;

            case HTTP_UPDATE_NO_UPDATES:
            Serial.println("[OTA] no updates");
            
            break;

            case HTTP_UPDATE_OK:
            #ifdef __debug
                Serial.println("[OTA] BIN updated");
            #endif
            ESP.restart();
            break;
        }
    }
}
