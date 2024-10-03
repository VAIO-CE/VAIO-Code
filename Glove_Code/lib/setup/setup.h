#ifndef SETUP_H
#define SETUP_H

#include <WiFi.h>
#include <esp_now.h>
#include <esp_arduino_version.h>
#include <constant.h>
#include <pin.h>
#include <gyro_sensor.h>
#include <vr3_sensor.h>
#include <button_sensor.h>

class Setup{
    public:
        static esp_now_peer_info_t peerInfo;
        static void ESPNOW();
        static void MPU6050();
        static void VR3();
        static void Button();
        static void InitialTask();
};
#endif