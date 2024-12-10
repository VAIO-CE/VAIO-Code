#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include <cstddef>
#include <stdint.h>

enum ESPNOW_Receive_Type {
  AUTO_CONTROL,
  GYRO_CONTROL,
  DS4_CONTROL,
};

enum ESPNOW_DataType {
  GYRO_SENSOR_DATA,
  SPEECH_DATA,
  VACUUM_DATA,
};

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

#endif 
