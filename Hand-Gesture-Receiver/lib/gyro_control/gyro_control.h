#ifndef GYRO_CONTROL_H
#define GYRO_CONTROL_H

#include "Arduino.h"
#include "constants.h"
#include "data_type.h"
#include "pin.h"

class GyroControl{
public:
    static struct GyroSensor_Data gyroSensor_Data;

    static void rotateMotor(int rightMotorSpeed, int leftMotorSpeed); 
    static void vTaskGestureControl(void * pvParameters);
};

#endif