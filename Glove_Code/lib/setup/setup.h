#ifndef SETUP_H
#define SETUP_H

#include "constant.h"
#include "pin.h"
#include "gyro_sensor.h"
#include "vacuum.h"
#include "speech_recognition.h"

#include <WiFi.h>
#include <esp_now.h>
#include <esp_arduino_version.h>
class Setup{
    public:
        static esp_now_peer_info_t peerInfo;
        static void ESPNOW();
        static void MPU6050();
        static void Buttons();
        static void SpeechRecognition();
        static void InitialTask();
};
#endif
