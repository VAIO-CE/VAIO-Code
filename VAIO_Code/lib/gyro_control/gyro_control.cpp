#include "gyro_control.h"

struct GyroSensor_Data GyroControl::gyroSensor_Data;

void GyroControl::vTaskGestureControl(void * pvParameters){
  int threshold = 50;
  
  while(true){
    digitalWrite(LED_BUILTIN, LOW);

    int rightMotorSpeed=0;
    int leftMotorSpeed=0;

    int mappedYValue = map(gyroSensor_Data.yAxisValue, 0, 254, -255, 255); 
    int mappedXValue = map(gyroSensor_Data.xAxisValue, 0, 254, -255, 255); 
    int motorDirection = 1;

    if (mappedYValue < - threshold)
    {
      motorDirection = -1;
    }
    
    if (abs(mappedYValue) < threshold) {
        mappedYValue = 0;
    }
    if (abs(mappedXValue) < threshold) {
      mappedXValue = 0;
    }

    rightMotorSpeed = abs(mappedYValue) - mappedXValue;
    leftMotorSpeed = abs(mappedYValue) + mappedXValue;

    rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);
    leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);

    rotateMotor(rightMotorSpeed * motorDirection, leftMotorSpeed * motorDirection);

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }

}

void GyroControl::rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  if (rightMotorSpeed < 0)
  {
    digitalWrite(motorRightPin1,LOW);
    digitalWrite(motorRightPin2,HIGH);    
  }
  else if (rightMotorSpeed > 0)
  {
    digitalWrite(motorRightPin1,HIGH);
    digitalWrite(motorRightPin2,LOW);      
  }
  else
  {
    digitalWrite(motorRightPin1,LOW);
    digitalWrite(motorRightPin2,LOW);      
  }
  
  if (leftMotorSpeed < 0)
  {
    digitalWrite(motorLeftPin1,LOW);
    digitalWrite(motorLeftPin2,HIGH);    
  }
  else if (leftMotorSpeed > 0)
  {
    digitalWrite(motorLeftPin1,HIGH);
    digitalWrite(motorLeftPin2,LOW);      
  }
  else
  {
    digitalWrite(motorLeftPin1,LOW);
    digitalWrite(motorLeftPin2,LOW);      
  }  

}
