#ifndef SETUP_H
#define SETUP_H

#include <WiFi.h>
#include <esp_now.h>
#include <esp_arduino_version.h>
#include <gyro_control.h>
#include <auto_control.h>
#include <pin.h>

class Setup{
    public:
        static void ESPNOW();
        static void Motors();
        static void Ultrasonic();
        static void Servo();
};
#endif