#include <gyro_control.h>

struct GyroSensor_Data GyroControl::gyroSensor_Data;

void GyroControl::vTaskGestureControl(void *pvParameters) {
  int threshold = 50;

  while (true) {

    int rightMotorSpeed = 0;
    int leftMotorSpeed = 0;

    int mappedYValue = map(gyroSensor_Data.yAxisValue, 0, 254, -255, 255);
    int mappedXValue = map(gyroSensor_Data.xAxisValue, 0, 254, -255, 255);
    int motorDirection = 1;

    if (mappedYValue < -threshold) {
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

    Motor::rotateMotor(rightMotorSpeed * motorDirection,leftMotorSpeed * motorDirection);

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
