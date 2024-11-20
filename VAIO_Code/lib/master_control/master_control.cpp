#include "master_control.h"

ControlState MasterControl::currentControlMode = ControlState::AUTO_CONTROL;
TaskHandle_t MasterControl::controlTaskHandle = NULL;

struct SpeechRecognition_Data MasterControl::speechRecognition_Data;

void MasterControl::ESPNOW_OnDataReceive(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  if (len < 1)
  {
    // Invalid data length
    return;
  }
  ESPNOW_DataType dataType = (ESPNOW_DataType)incomingData[0];
  const uint8_t *data = incomingData + 1;
  int dataLen = len - 1;

  switch (dataType)
  {
  case GYRO_SENSOR_DATA:
    Serial.println("Receive Gyro Data");
    memcpy(&GyroControl::gyroSensor_Data, data, sizeof(GyroSensor_Data));
    break;
  case BUTTON_DATA:
    handleButtonPress(data);
    break;
  case SPEECH_DATA:
    Serial.println("Receive Speech Data");
    memcpy(&MasterControl::speechRecognition_Data, data, sizeof(SpeechRecognition_Data));
    handleSpeechCommand();
    break;
  default:
    // Unknown data type
    break;
  }
}

void MasterControl::setControlMode(ControlState mode)
{
  switch (mode)
  {
  case ControlState::AUTO_CONTROL:
    // Stop the current task
    vTaskDelete(controlTaskHandle);
    // Start the auto control task
    xTaskCreatePinnedToCore(AutoControl::vTaskAutoControl, "Auto Control", STACK_SIZE, NULL, 1, &controlTaskHandle, 0);
    break;
  case ControlState::GYRO_CONTROL:
    // Stop the current task
    vTaskDelete(controlTaskHandle);
    // Start the gyro control task
    xTaskCreatePinnedToCore(GyroControl::vTaskGestureControl, "Gyro Control", STACK_SIZE, NULL, 1, &controlTaskHandle, 0);
    break;
  case ControlState::DS4_CONTROL:
    // Stop the current task
    vTaskDelete(controlTaskHandle);
    // Start the DS4 control task
    xTaskCreatePinnedToCore(DS4Control::vTaskDS4Control, "DS4 Control", 2 * STACK_SIZE, NULL, 1, &controlTaskHandle, 0);
    break;
  default:
    // Handle unknown control mode
    break;
  }
  currentControlMode = mode;
}

void MasterControl::handleButtonPress(const uint8_t *data)
{
  // For example, switch control modes based on button press data
  if (data[0] == 0x01)
  { // Button press data for switching to gyro control
    setControlMode(ControlState::GYRO_CONTROL);
  }
  else if (data[0] == 0x02)
  { // Button press data for switching to auto control
    setControlMode(ControlState::AUTO_CONTROL);
  }
  else if (data[0] == 0x03)
  {
    // Button press data for switching to DS4 control
    setControlMode(ControlState::DS4_CONTROL);
  }
}

void MasterControl::handleSpeechCommand(){
  if (speechRecognition_Data.control > 0.7){
      Serial.println("Heard: Control");
      setControlMode(ControlState::DS4_CONTROL);
  }
  if (speechRecognition_Data.hand > 0.7){
      Serial.println("Heard: Hand");
      setControlMode(ControlState::GYRO_CONTROL);
  }
  if (speechRecognition_Data.move > 0.55){
      Serial.println("Heard: Move");
      setControlMode(ControlState::AUTO_CONTROL);
  }
}