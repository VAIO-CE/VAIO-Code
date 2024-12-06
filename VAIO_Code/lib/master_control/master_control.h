#ifndef MASTER_CONTROL_H
#define MASTER_CONTROL_H

#include "pin.h"
#include "data_type.h"
#include "ds4_control.h"
#include "auto_control.h"
#include "gyro_control.h"

#include <Arduino.h>
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Preferences.h>
#include <ps4Controller.h>

// Define ENUM for control states
enum class ControlState
{
  AUTO_CONTROL,
  GYRO_CONTROL,
  DS4_CONTROL,
};

class MasterControl
{
private:
  static struct SpeechRecognition_Data speechRecognition_Data;

  static void init();
  static void handleButtons(const uint8_t *data);
  static void handleSpeechCommand();

public:
  static TaskHandle_t controlTaskHandle;
  static void setControlMode(ControlState mode);
  static void changeLEDIndicator(ControlState mode);
  static void ESPNOW_OnDataReceive(const uint8_t *mac, const uint8_t *incomingData, int len);
};

#endif
