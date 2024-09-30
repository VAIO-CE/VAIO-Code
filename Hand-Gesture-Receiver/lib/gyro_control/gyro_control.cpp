#include "gyro_control.h"

struct GyroSensor_Data GyroControl::gyroSensor_Data;

void GyroControl::OnDataReceive(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&gyroSensor_Data, incomingData, sizeof(GyroSensor_Data));

  // Create FreeRTOS Task
  xTaskCreatePinnedToCore(
    vTaskHandGestureControl,
    "Gesture Control",
    STACK_SIZE, // Stack
    NULL, // Parameter to pass function
    1, // Task Priority
    NULL, // Task Handle
    0 // CPU core
  );
}

void GyroControl::vTaskHandGestureControl(void * pvParameters){
  int rightMotorSpeed=0;
  int leftMotorSpeed=0;

  int mappedYValue = map(gyroSensor_Data.yAxisValue, 0, 254, -255, 255); 
  int mappedXValue = map(gyroSensor_Data.xAxisValue, 0, 254, -255, 255); 
  int motorDirection = 1;

  if (mappedYValue < 0)
  {
    motorDirection = -1;
  }

  rightMotorSpeed = abs(mappedYValue) - mappedXValue;
  leftMotorSpeed = abs(mappedYValue) + mappedXValue;

  rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);
  leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);
  
  rotateMotor(rightMotorSpeed * motorDirection, leftMotorSpeed * motorDirection);

  vTaskDelete(NULL); // Delete this Task
}

void GyroControl::rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  if (rightMotorSpeed < 0)
  {
    Serial.println("Right Motor Pin  1 : LOW");
    Serial.println("Right Motor Pin  2 : HIGH"); 
  }
  else if (rightMotorSpeed > 0)
  {
    Serial.println("Right Motor Pin  1 : HIGH");
    Serial.println("Right Motor Pin  2 : LOW");   
  }
  else
  {
    Serial.println("Right Motor Pin  1 : LOW");
    Serial.println("Right Motor Pin  2 : LOW");    
  }

  Serial.println();   
  
  if (leftMotorSpeed < 0)
  {
    Serial.println("Left Motor Pin  1 : LOW");
    Serial.println("Left Motor Pin  2 : HIGH");  
  }
  else if (leftMotorSpeed > 0)
  {
    Serial.println("Left Motor Pin  1 : HIGH");
    Serial.println("Left Motor Pin  2 : LOW");       
  }
  else
  {
    Serial.println("Left Motor Pin  1 : LOW");
    Serial.println("Left Motor Pin  2 : LOW");    
  }

  Serial.println();     

  // analogWrite(enableRightMotor, abs(rightMotorSpeed));
  // analogWrite(enableLeftMotor, abs(leftMotorSpeed));    
}
