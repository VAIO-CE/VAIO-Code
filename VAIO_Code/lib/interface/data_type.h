#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include <cstdint>

enum ESPNOW_DataType {
  GYRO_SENSOR_DATA,
  SPEECH_DATA,
  VACUUM_DATA,
};

enum ControlState {
  AUTO_CONTROL,
  GYRO_CONTROL,
  DS4_CONTROL,
};

struct Vacuum_Data
{
  uint8_t data;    
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
