#ifndef SETUP_H
#define SETUP_H

#include "I2Cdev.h"
#include "constant.h"
#include "data_types.h"
#include "gyro_sensor.h"
#include "pin.h"
#include "speech_recognition.h"
#include "state_control.h"
#include "vacuum.h"

#include <WiFi.h>
#include <esp_arduino_version.h>
#include <esp_now.h>

class Setup {
public:
  static inline esp_now_peer_info_t peerInfo{};
  static void ESPNOW();
  static void MPU6050();
  static void Buttons();
  static void SpeechRecognition();
  static void InitialTask();
};
#endif
