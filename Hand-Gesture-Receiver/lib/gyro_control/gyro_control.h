#ifndef GYRO_CONTROL_H
#define GYRO_CONTROL_H

#include "Arduino.h"
#include "constants.h"
#include "data_type.h"

class GyroControl{
public:
    static struct GyroSensor_Data gyroSensor_Data;

    static void OnDataReceive(const uint8_t * mac, const uint8_t *incomingData, int len);
    static void rotateMotor(int rightMotorSpeed, int leftMotorSpeed); 
    static void vTaskHandGestureControl(void * pvParameters);
};

#endif