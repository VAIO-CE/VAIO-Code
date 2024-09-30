#ifndef MASTER_CONTROL_H
#define MASTER_CONTROL_H

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "auto_control.h"
#include "gyro_control.h"

// Define ENUM for control states
enum class ControlState {
  AUTO_CONTROL,
  GYRO_CONTROL,
  DS4_CONTROL,
};

class MasterControl{
public:
    static ControlState currentControlMode;
    static TaskHandle_t controlTaskHandle;
    
    static void init();
    static void setControlMode(ControlState mode);
    static void handleButtonPress(const uint8_t* data) ;
    static void ESPNOW_OnDataReceive(const uint8_t * mac, const uint8_t *incomingData, int len);

};

#endif
