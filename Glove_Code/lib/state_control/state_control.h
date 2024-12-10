
#ifndef STATECONTROL_H
#define STATECONTROL_H

#include "constant.h"
#include "data_types.h"
#include "gyro_sensor.h"

#include <Arduino.h>
#include <esp_arduino_version.h>
#include <esp_now.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdint.h>

class StateControl {
public:
  static TaskHandle_t gyroTaskHandle;
  static void ESPNOW_OnDataReceive(const uint8_t *mac,
                                   const uint8_t *incomingData, int len);
};
#endif
