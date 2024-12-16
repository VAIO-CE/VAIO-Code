#ifndef SETUP_H
#define SETUP_H

#include "secrets.h"
#include "pin.h"
#include "constants.h"
#include "data_type.h"
#include "web_server.h"
#include "master_control.h"
#include "auto_control.h"
#include "ds4_control.h"
#include "gyro_control.h"

#include <ESPmDNS.h>
#include <WiFi.h>
#include <esp32-hal-ledc.h>
#include <esp_arduino_version.h>
#include <esp_now.h>

class Setup
{
public:
  static inline esp_now_peer_info_t peerInfo{};
  static void Wifi();
  static void ESPNOW();
  static void WebServer();
  static void LEDIndicators();
  static void Motors();
  static void Ultrasonic();
  static void Servo();
  static void DS4();
  static void InitialTask();
};
#endif
