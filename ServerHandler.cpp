#include "ServerHandler.h"
#include <Update.h>
#include "index.h"

//unmark following line to enable debug mode
#define __debug

void ServerHandler::begin()
{
    #ifdef __debug  
        Serial.println("[Server]: init");
    #endif
    server.onNotFound([this]() {handleNotFound(); });
    server.on("/",HTTP_GET, [this]() { handleRoot();});
    server.on("/repo/install/request",HTTP_POST,[this]() { handleInstallFromRepo();});
    server.on("/repo/info/request",HTTP_POST, [this]() { handleRepoInformation();});
    server.on("/repo/info",HTTP_GET, [this]() { checkRepoInformation();});
    server.on("/repo/list/request",HTTP_GET, [this]() { handleRepoList();});
    server.on("/repo/list",HTTP_GET, [this]() { checkRepoList();});
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
    repoListState = 2;
    server.send(200, "text/plain", "ok");
}

void ServerHandler::checkRepoList() {
    if (repoListState == 1) {
        server.send(200, "text/json",repoList);
    } else {
        server.send(200, "text/plain",String(repoListState));
    }
}

void ServerHandler::handleRepoInformation()
{
    if (server.hasArg("repo")) {
        repoPath = server.arg("repo") + "list.json";
        repoInformationState = 2;
        server.send(200, "text/plain", "ok");
        return;
    }
    server.send(503, "text/plain", "Parameters missing");
}

void ServerHandler::checkRepoInformation() {
    if (repoInformationState == 1) {
        server.send(200, "text/json",repoInformation);
    } else {
        server.send(200, "text/plain",String(repoInformationState));
    }
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

void ServerHandler::handleInstallFromRepo() {
    if (server.hasArg("spiffsPath")) {
        spiffPath = server.arg("spiffsPath");
        repoInstallationState = 2;
    }
    if (server.hasArg("binPath")) {
        binPath = server.arg("binPath");
        repoInstallationState = 2;
    }
    server.send(200, "text/plain", "ok");
}
