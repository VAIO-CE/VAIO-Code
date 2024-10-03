#ifndef SETUP_H
#define SETUP_H

#include <WiFi.h>
#include <esp_now.h>
#include <esp_arduino_version.h>
#include <gyro_control.h>
#include <auto_control.h>
#include <master_control.h>
#include <web_server.h>
#include <ESPmDNS.h>
#include <pin.h>
#include <ds4_control.h>
#include <Preferences.h>

class Setup
{
public:
    static void Wifi();
    static void ESPNOW();
    static void WebServer();
    static void Motors();
    static void Ultrasonic();
    static void Servo();
    static void DS4();
    static void InitialTask();
};
#endif