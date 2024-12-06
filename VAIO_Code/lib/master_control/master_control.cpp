#include "master_control.h"

TaskHandle_t MasterControl::controlTaskHandle = NULL;

struct SpeechRecognition_Data MasterControl::speechRecognition_Data;

void MasterControl::ESPNOW_OnDataReceive(const uint8_t *mac,
                                         const uint8_t *incomingData, int len) {
  if (len < 1) {
    // Invalid data length
    return;
  }
  ESPNOW_DataType dataType = (ESPNOW_DataType)incomingData[0];
  const uint8_t *data = incomingData + 1;
  int dataLen = len - 1;

  digitalWrite(LED_BUILTIN, HIGH);
  switch (dataType) {
  case GYRO_SENSOR_DATA:
    memcpy(&GyroControl::gyroSensor_Data, data, sizeof(GyroSensor_Data));
    break;
  case SPEECH_DATA:
    memcpy(&MasterControl::speechRecognition_Data, data,
           sizeof(SpeechRecognition_Data));
    handleSpeechCommand();
    break;
  }
//  char *taskName = pcTaskGetTaskName(controlTaskHandle);
//  printf("Control Changed to : %s\n", taskName);
}

void MasterControl::setControlMode(ControlState mode) {
  switch (mode) {
  case ControlState::AUTO_CONTROL:
    digitalWrite(autoLEDPin, HIGH);
    digitalWrite(gyroLEDPin, LOW);
    digitalWrite(ds4LEDPin, LOW);
    vTaskDelete(controlTaskHandle);
    xTaskCreatePinnedToCore(AutoControl::vTaskAutoControl, "Auto Control",
                            STACK_SIZE, NULL, 1, &controlTaskHandle, 0);
    break;
  case ControlState::GYRO_CONTROL:
    digitalWrite(autoLEDPin, LOW);
    digitalWrite(gyroLEDPin, HIGH);
    digitalWrite(ds4LEDPin, LOW);
    vTaskDelete(controlTaskHandle);
    xTaskCreatePinnedToCore(GyroControl::vTaskGestureControl, "Gyro Control",
                            STACK_SIZE, NULL, 1, &controlTaskHandle, 0);
    break;
  case ControlState::DS4_CONTROL:
    digitalWrite(autoLEDPin, LOW);
    digitalWrite(gyroLEDPin, HIGH);
    digitalWrite(ds4LEDPin, LOW);
    vTaskDelete(controlTaskHandle);
    xTaskCreatePinnedToCore(DS4Control::vTaskDS4Control, "DS4 Control",
                            2 * STACK_SIZE, NULL, 1, &controlTaskHandle, 0);
    break;
  }
}

void MasterControl::changeLEDIndicator(ControlState mode) {
  switch (mode) {
  case ControlState::AUTO_CONTROL:
    digitalWrite(autoLEDPin, HIGH);
    digitalWrite(gyroLEDPin, LOW);
    digitalWrite(ds4LEDPin, LOW);
    break;
  case ControlState::GYRO_CONTROL:
    digitalWrite(autoLEDPin, LOW);
    digitalWrite(gyroLEDPin, HIGH);
    digitalWrite(ds4LEDPin, LOW);
    break;
  case ControlState::DS4_CONTROL:
    digitalWrite(autoLEDPin, LOW);
    digitalWrite(gyroLEDPin, HIGH);
    digitalWrite(ds4LEDPin, LOW);
    break;
  }
}

void MasterControl::handleSpeechCommand() {
  if (speechRecognition_Data.control > 0.5) {
    changeLEDIndicator(ControlState::DS4_CONTROL);
    setControlMode(ControlState::DS4_CONTROL);
  }
  if (speechRecognition_Data.hand > 0.5) {
    changeLEDIndicator(ControlState::GYRO_CONTROL);
    setControlMode(ControlState::GYRO_CONTROL);
  }
  if (speechRecognition_Data.move > 0.5) {
    changeLEDIndicator(ControlState::AUTO_CONTROL);
    setControlMode(ControlState::AUTO_CONTROL);
  }
}
