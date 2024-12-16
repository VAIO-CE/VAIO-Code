#ifndef MASTER_CONTROL_H
#define MASTER_CONTROL_H

#include "pin.h"
#include "data_type.h"
#include "ds4_control.h"
#include "auto_control.h"
#include "gyro_control.h"

#include <Arduino.h>
#include <stdint.h>
#include <esp_now.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Preferences.h>
#include <ps4Controller.h>

class MasterControl
{
private:
  static struct SpeechRecognition_Data speechRecognition_Data;

  static void init();
  static void handleVacuumToggle(const uint8_t *data);
  static void handleSpeechCommand();

public:
  static inline uint8_t controlData[1]{GYRO_CONTROL};
  static inline TaskHandle_t controlTaskHandle{NULL};
  static void setControlMode(ControlState mode);
  static void changeLEDIndicator(ControlState mode);
  static void ESPNOW_OnDataReceive(const uint8_t *mac, const uint8_t *incomingData, int len);
  static void ESPNOW_OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
};

#endif
