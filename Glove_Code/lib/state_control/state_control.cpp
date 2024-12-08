#include "state_control.h"

TaskHandle_t StateControl::gyroTaskHandle = NULL;

void StateControl::ESPNOW_OnDataReceive(const uint8_t *mac,
                                        const uint8_t *incomingData, int len) {

  ESPNOW_Receive_Type controlType = (ESPNOW_Receive_Type)incomingData[0];

  switch (controlType) {
  case GYRO_CONTROL:
    printf("VAIO State : Gyro Control\n");
    if (gyroTaskHandle == NULL) {

      printf("Starting Gyro Task\n");
      xTaskCreatePinnedToCore(GyroSensor::vTaskGestureControl,
                              "Hand Gesture Control",
                              STACK_SIZE,      // Stack
                              NULL,            // Parameter to pass function
                              1,               // Task Priority
                              &gyroTaskHandle, // Task Handle
                              0                // CPU core
      );
    } else
      printf("Gyro Task already created\n");
    break;

  case AUTO_CONTROL:
    printf("VAIO State : Auto Control\n");

    if (gyroTaskHandle != NULL) {
      printf("Deleting Gyro Task\n");
      vTaskDelete(gyroTaskHandle);
      gyroTaskHandle = NULL; // Reset the task handle
    } else
      printf("Gyro Task already deleted\n");

    break;
  case DS4_CONTROL:
    printf("VAIO State : DS4 Control\n");
    if (gyroTaskHandle != NULL) {
      printf("Deleting Gyro Task\n");
      vTaskDelete(gyroTaskHandle);
      gyroTaskHandle = NULL; // Reset the task handle
    } else
      printf("Gyro Task already deleted\n");
    break;
  }
}
