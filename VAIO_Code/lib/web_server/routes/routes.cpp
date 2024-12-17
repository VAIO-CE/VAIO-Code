#include "routes.h"

Preferences pref;

// @GET("/")
void handleRoot(AsyncWebServerRequest *req) {
  req->send(200, "text/plain", "Hello, World!");
}

// @GET("/mac")
void handleGetSavedMac(AsyncWebServerRequest *req) {
  pref.begin("DS4-NVS", false);
  String jsonResponse =
      "{\"savedMac\":\"" + pref.getString("btmac", "empty") + "\"}";
  req->send(200, "application/json", jsonResponse);
  pref.end();
}

// Handles all post requests
void handleOnBodyPost(AsyncWebServerRequest *req, uint8_t *data, size_t len,
                      size_t index, size_t total) {
  // @POST("/mac")
  if (req->url() == "/mac") {
    StaticJsonDocument<200> doc;
    DeserializationError err = deserializeJson(doc, data);

    if (err) {
      req->send(400, "application/json",
                "{\"message\":\"Invalid JSON format\"}");
      return;
    }

    const char *macAddress = doc["mac"];

    if (macAddress != nullptr) {
      pref.begin("DS4-NVS", false);
      pref.putString("btmac", macAddress);

      String jsonResponse =
          "{\"message\":\"MAC address stored successfully\", \"mac\":\"" +
          pref.getString("btmac", "empty") + "\"}";

      req->send(200, "application/json", jsonResponse);
      pref.end();
    } else {
      req->send(400, "application/json",
                "{\"message\":\"MAC address not found in request\"}");
    }
  } else if (req->url() == "/upgrade") {
    // To-do
    // Implement ESP32 firmware upgrade
    // Over the air
  } else {
    req->send(400, "application/json", "{\"message\":\"No data received\"}");
  }
}

// @POST("/changeControlState")
void handleChangeState(AsyncWebServerRequest *req) {
  String state = req->arg("state");
  ControlState newControlState;

  if (state == "AUTO_CONTROL")
    newControlState = ControlState::AUTO_CONTROL;
  else if (state == "GYRO_CONTROL")
    newControlState = ControlState::GYRO_CONTROL;
  else if (state == "DS4_CONTROL")
    newControlState = ControlState::DS4_CONTROL;
  else
    req->send(400, "text/plain", "Bad request");

  MasterControl::setControlMode(newControlState);
  req->send(200, "text/plain", "Control state changed");
}

// 404 Callback
void handleNotFound(AsyncWebServerRequest *req) {
  req->send(400, "application/json", "{\"message\":\"Not found\"}");
}
