#define byte uint8_t

/// @brief Gyroscope data from ESP-NOW
struct GyroSensor_Data
{
  byte xAxisValue;    
  byte yAxisValue;
};