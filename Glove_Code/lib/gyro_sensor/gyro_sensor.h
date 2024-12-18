#ifndef GYROSENSOR_H
#define GYROSENSOR_H

#include "data_types.h"
#include "constant.h"
#include "pin.h"
#include "I2Cdev.h"

#include <MPU6050_6Axis_MotionApps20.h>
#include <esp_now.h>

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

class GyroSensor{
    public:
        static struct GyroSensor_Data gyroSensor_Data;
        static bool dmpReady;  // set true if DMP init was successful 
        static MPU6050 mpu;
        static uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
        static uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
        static uint8_t fifoBuffer[64]; // FIFO storage buffer
        static Quaternion q;           // [w, x, y, z]         quaternion container
        static VectorFloat gravity;    // [x, y, z]            gravity vector
        static float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

        static void vTaskGestureControl(void * pvParameters);
};
#endif
