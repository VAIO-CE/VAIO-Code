#include "master_control.h"

ControlState MasterControl::currentControlMode = ControlState::AUTO_CONTROL;
ControlState MasterControl::previousControlMode = currentControlMode;
TaskHandle_t MasterControl::controlTaskHandle = NULL;

bool MasterControl::isInterchange = false;

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
  
  digitalWrite(LED_BUILTIN, HIGH);
  switch (dataType)
  {
  case GYRO_SENSOR_DATA:
    memcpy(&GyroControl::gyroSensor_Data, data, sizeof(GyroSensor_Data));
    break;
  case SPEECH_DATA:
    memcpy(&MasterControl::speechRecognition_Data, data, sizeof(SpeechRecognition_Data));
    handleSpeechCommand(isInterchange);
    break;
  default:
    // Unknown data type
    break;
  }
  char* taskName = pcTaskGetTaskName(controlTaskHandle);
  printf("Control Changed to : %s\n", taskName);
  // Serial.println("Current STate: ", currentControlMode);
  // Serial.println("Previous state: ", previousControlMode);
}

void MasterControl::setControlMode(ControlState mode)
{
  // previousControlMode = currentControlMode;
  switch (mode)
  {
  case ControlState::AUTO_CONTROL:
    // Stop the current task
    vTaskDelete(controlTaskHandle);
    isInterchange = false;
    // Start the auto control task
    xTaskCreatePinnedToCore(AutoControl::vTaskAutoControl, "Auto Control", STACK_SIZE, NULL, 1, &controlTaskHandle, 0);
    break;
  case ControlState::GYRO_CONTROL:
    // Stop the current task
    vTaskDelete(controlTaskHandle);
    isInterchange = false;
    // Start the gyro control task
    xTaskCreatePinnedToCore(GyroControl::vTaskGestureControl, "Gyro Control", STACK_SIZE, NULL, 1, &controlTaskHandle, 0);
    break;
  case ControlState::DS4_CONTROL:
    // Stop the current task
    vTaskDelete(controlTaskHandle);
    isInterchange = false;
    // Start the DS4 control task
    xTaskCreatePinnedToCore(DS4Control::vTaskDS4Control, "DS4 Control", 2 * STACK_SIZE, NULL, 1, &controlTaskHandle, 0);
    break;
  case ControlState::STOP_STATE:
    vTaskDelete(controlTaskHandle);
    isInterchange = false;
    // idle task
    xTaskCreatePinnedToCore(MasterControl::vTaskStop, "Stop State", STACK_SIZE, NULL, 1, &controlTaskHandle, 0);
    break;
  case ControlState::INTERCHANGE_STATE:
    // vTaskDelete(controlTaskHandle);
    // idle task
    Serial.println("Hearing command...");
    isInterchange = true;
    previousControlMode = currentControlMode;
    xTaskCreatePinnedToCore(MasterControl::vTaskInterchange, "Interchange State", STACK_SIZE, NULL, 1, &controlTaskHandle, 0);
    break;
  default:
    // Handle unknown control mode
    break;
  }
  currentControlMode = mode;
}


void MasterControl::handleSpeechCommand(bool isInterchange){
  if (isInterchange){
      if (speechRecognition_Data.control > 0.7){
      Serial.println("Heard: Control");
      setControlMode(ControlState::DS4_CONTROL);
      }
      if (speechRecognition_Data.hand > 0.7){
          Serial.println("Heard: Hand");
          setControlMode(ControlState::GYRO_CONTROL);
      }
      if (speechRecognition_Data.move > 0.7){
          Serial.println("Heard: Move");
          setControlMode(ControlState::AUTO_CONTROL);
      }
      if (speechRecognition_Data.off > 0.7){
        Serial.println("Heard: Off, Cancel change state");
        setControlMode(previousControlMode);
      }
  }
  else {
    if (speechRecognition_Data.on > 0.7){
        Serial.println("Heard: On, Will change state...");
        setControlMode(ControlState::INTERCHANGE_STATE);
    }

    if (speechRecognition_Data.stop > 0.7){
        Serial.println("Heard: Stop");
        setControlMode(ControlState::STOP_STATE);
    }
  }


}


void MasterControl::vTaskStop(void *pvParameters){
  while(true){
    digitalWrite(motorRightPin1, LOW);
    digitalWrite(motorRightPin2, LOW);
    digitalWrite(motorLeftPin1, LOW);
    digitalWrite(motorLeftPin2, LOW);

    vTaskDelay(portTICK_PERIOD_MS / 100);
  }
}

void MasterControl::vTaskInterchange(void *pvParameters){
  int counter = 0;
  while(true){
    counter = counter+1;
    if (counter < 5){
      isInterchange = false;
      setControlMode(previousControlMode);
      vTaskDelete(NULL);
    }
    Serial.print("Count :");
    Serial.println(counter);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
