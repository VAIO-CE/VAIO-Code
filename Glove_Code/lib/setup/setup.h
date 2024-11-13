#ifndef SETUP_H
#define SETUP_H

#include <WiFi.h>
#include <esp_now.h>
#include <esp_arduino_version.h>
#include <constant.h>
#include <pin.h>
#include <gyro_sensor.h>
#include <button_sensor.h>
#include <speech_recognition.h>

class Setup{
    public:
        static esp_now_peer_info_t peerInfo;
        static void ESPNOW();
        static void MPU6050();
        static void SpeechRecognition();
        static void Button();
        static void InitialTask();
};
#endif