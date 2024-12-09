#include "master_control.h"
#include "esp32-hal-gpio.h"
#include "pin.h"

TaskHandle_t MasterControl::controlTaskHandle = NULL;

struct SpeechRecognition_Data MasterControl::speechRecognition_Data;

void MasterControl::ESPNOW_OnDataReceive(const uint8_t *mac,
                                         const uint8_t *incomingData, int len)
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
    memcpy(&GyroControl::gyroSensor_Data, data, sizeof(GyroSensor_Data));
    break;
  case SPEECH_DATA:
    memcpy(&MasterControl::speechRecognition_Data, data,
           sizeof(SpeechRecognition_Data));
    handleSpeechCommand();
    break;
  case VACUUM_DATA:
    handleVacuumToggle(data);
    break;
  }
  char *taskName = pcTaskGetTaskName(controlTaskHandle);
  printf("Control Changed to : %s\n", taskName);
}

void MasterControl::handleVacuumToggle(const uint8_t *data)
{
  Serial.print("test_sent");
  if (data[0] == 0x01)
  {
    Serial.println("Turn Off Vacuum");
    digitalWrite(vacuumPin, LOW);
  }
  else if (data[0] == 0x02)
  {
    Serial.println("Turn On Vacuum");
    digitalWrite(vacuumPin, HIGH);
  }
}

void MasterControl::setControlMode(ControlState mode)
{
  Motor::rotateMotor(0, 0);
  switch (mode)
  {
  case ControlState::AUTO_CONTROL:
    vTaskDelete(controlTaskHandle);
    xTaskCreatePinnedToCore(AutoControl::vTaskAutoControl, "Auto Control",
                            STACK_SIZE, NULL, 1, &controlTaskHandle, 0);
    break;
  case ControlState::GYRO_CONTROL:
    vTaskDelete(controlTaskHandle);
    xTaskCreatePinnedToCore(GyroControl::vTaskGestureControl, "Gyro Control",
                            STACK_SIZE, NULL, 1, &controlTaskHandle, 0);
    break;
    // case ControlState::DS4_CONTROL:
    //   DS4Control::ps4.begin("D0:27:88:51:4C:50");
    //   vTaskDelete(controlTaskHandle);
    //   xTaskCreatePinnedToCore(DS4Control::vTaskDS4Control, "DS4 Control",
    //                           2 * STACK_SIZE, NULL, 2, &controlTaskHandle, 0);
    //   break;
  }
}

void MasterControl::changeLEDIndicator(ControlState mode)
{
  digitalWrite(autoLEDPin, LOW);
  digitalWrite(gyroLEDPin, LOW);
  digitalWrite(ds4LEDPin, LOW);

  switch (mode)
  {
  case ControlState::AUTO_CONTROL:
    digitalWrite(autoLEDPin, HIGH);
    break;
  case ControlState::GYRO_CONTROL:
    digitalWrite(gyroLEDPin, HIGH);
    break;
    // case ControlState::DS4_CONTROL:
    //   digitalWrite(ds4LEDPin, HIGH);
    //   break;
  }
}

void MasterControl::handleSpeechCommand()
{
  // if (speechRecognition_Data.control > 0.5)
  // {
  //   changeLEDIndicator(ControlState::DS4_CONTROL);
  //   setControlMode(ControlState::DS4_CONTROL);
  // }
  if (speechRecognition_Data.hand > 0.5)
  {
    changeLEDIndicator(ControlState::GYRO_CONTROL);
    setControlMode(ControlState::GYRO_CONTROL);
  }
  if (speechRecognition_Data.move > 0.5)
  {
    changeLEDIndicator(ControlState::AUTO_CONTROL);
    setControlMode(ControlState::AUTO_CONTROL);
  }
}
