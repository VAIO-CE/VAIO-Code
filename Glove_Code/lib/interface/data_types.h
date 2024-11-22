#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include <cstddef>

enum ESPNOW_DataType {
  GYRO_SENSOR_DATA,
  SPEECH_DATA,
};

/// @brief Gyroscope data from ESP-NOW
struct GyroSensor_Data
{
  uint8_t xAxisValue;    
  uint8_t yAxisValue;
};

struct SpeechRecognition_Data
{
  float move;
  float control;
  float hand;
  float on;
  float off;
};

/// @brief VR3 data from ESP-NOW
struct Button_Data
{
  uint8_t button1;    
  uint8_t button2;    
};

#endif 