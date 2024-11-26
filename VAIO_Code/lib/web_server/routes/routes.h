#ifndef ROUTES_H
#define ROUTES_H

#include <Arduino.h>
#include <master_control.h>
#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"
#include "Preferences.h"

void handleRoot(AsyncWebServerRequest *req);
void handleGetSavedMac(AsyncWebServerRequest *req);
void handleOnBodyPost(AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t index, size_t total);
void handleChangeState(AsyncWebServerRequest *req);
void handleNotFound(AsyncWebServerRequest *req);

#endif