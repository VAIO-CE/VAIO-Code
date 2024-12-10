#include "master_control.h"

uint8_t MasterControl::controlData[1] = {GYRO_CONTROL};
TaskHandle_t MasterControl::controlTaskHandle = NULL;
ControlState currentState = GYRO_CONTROL;
ControlState prevState = currentState;

struct SpeechRecognition_Data MasterControl::speechRecognition_Data;

void MasterControl::ESPNOW_OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  if (status == ESP_NOW_SEND_FAIL)
  {
    Serial.println("ESPNow Delivery --> Glove: Fail");
    Serial.println("Resending Packet to Glove...");
    esp_now_send(GloveAddress, (uint8_t *)&controlData, sizeof(controlData));
  }
  else
  {
    Serial.println("ESPNow Delivery --> Glove: Success");
  }
}

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
    Serial.println("Speech Data Received");
    handleSpeechCommand();
    break;
  case VACUUM_DATA:
    handleVacuumToggle(data);
    break;
  default:
    Serial.println("Unknown Data\n");
  }

  char *taskName = pcTaskGetTaskName(MasterControl::controlTaskHandle);
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

  // if (prevState == ControlState::DS4_CONTROL)
  // {
  //   DS4Control::ps4.end();
  // }

  switch (mode)
  {
  case ControlState::AUTO_CONTROL:
    controlData[0] = AUTO_CONTROL;
    vTaskDelete(controlTaskHandle);
    xTaskCreatePinnedToCore(AutoControl::vTaskAutoControl, "Auto Control",
                            STACK_SIZE, NULL, 1, &controlTaskHandle, 0);
    break;
  case GYRO_CONTROL:
    controlData[0] = GYRO_CONTROL;
    vTaskDelete(controlTaskHandle);
    xTaskCreatePinnedToCore(GyroControl::vTaskGestureControl, "Gyro Control",
                            STACK_SIZE, NULL, 1, &controlTaskHandle, 0);
    break;
  case ControlState::DS4_CONTROL:
    controlData[0] = DS4_CONTROL;
    vTaskDelete(controlTaskHandle);
    Serial.println("Ganti");
    xTaskCreatePinnedToCore(DS4Control::vTaskDS4Setup, "DS4 Task Setup",
                            STACK_SIZE * 2, NULL, 1, NULL, 0);
    // xTaskCreatePinnedToCore(DS4Control::vTaskDS4Control, "DS4 Control",
    //                         2 * STACK_SIZE, NULL, 1, &controlTaskHandle, 0);
    Serial.println("Tahap 4");
    break;
  }

  currentState = mode;
  Serial.println("Send Change State to Glove");
  esp_now_send(GloveAddress, (uint8_t *)&controlData,
               sizeof(controlData)); // Send state changes to Glove
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
  case ControlState::DS4_CONTROL:
    digitalWrite(ds4LEDPin, HIGH);
    break;
  }
}

void MasterControl::handleSpeechCommand()
{
  prevState = currentState;
  if (speechRecognition_Data.control > 0.5)
  {
    changeLEDIndicator(ControlState::DS4_CONTROL);
    setControlMode(ControlState::DS4_CONTROL);
  }
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
