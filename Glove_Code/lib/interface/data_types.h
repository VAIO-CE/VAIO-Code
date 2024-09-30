#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include <cstddef>

enum ESPNOW_DataType {
  GYRO_SENSOR_DATA,
  BUTTON_DATA
};

/// @brief Gyroscope data from ESP-NOW
struct GyroSensor_Data
{
  byte xAxisValue;    
  byte yAxisValue;
};

/// @brief VR3 data from ESP-NOW
struct VR3Sensor_Data
{
  byte skibidiToilet1;    
  byte skibidiToilet2;
};

/// @brief VR3 data from ESP-NOW
struct Button_Data
{
  byte button1;    
  byte button2;    
};

#endif 