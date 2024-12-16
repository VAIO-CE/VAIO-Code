#ifndef GYRO_CONTROL_H
#define GYRO_CONTROL_H

#include "data_type.h"
#include "motor.h"

#include <Arduino.h>
class GyroControl{
public:
    static inline struct GyroSensor_Data gyroSensor_Data{};

    static void vTaskGestureControl(void * pvParameters);
};

#endif
