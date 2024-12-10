#include "gyro_sensor.h"

struct GyroSensor_Data GyroSensor::gyroSensor_Data;
bool GyroSensor::dmpReady = false; // set true if DMP init was successful
MPU6050 GyroSensor::mpu;
uint8_t GyroSensor::devStatus; // return status after each device operation (0 =
                               // success, !0 = error)
uint16_t
    GyroSensor::packetSize;         // expected DMP packet size (default is 42 bytes)
uint8_t GyroSensor::fifoBuffer[64]; // FIFO storage buffer
Quaternion GyroSensor::q;           // [w, x, y, z]         quaternion container
VectorFloat GyroSensor::gravity;    // [x, y, z]            gravity vector
float GyroSensor::ypr[3];

uint8_t buttonToggleData = 0x01;

void GyroSensor::vTaskGestureControl(void *pvParameters)
{
  while (1)
  {
    // if programming failed, don't try to do anything
    if (!dmpReady)
      return;

    // read a packet from FIFO. Get the Latest packet
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
    {
      // display Euler angles in degrees
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

      int xAxisValue = constrain(ypr[2] * 180 / M_PI, -90, 90);
      int yAxisValue = constrain(ypr[1] * 180 / M_PI, -90, 90);
      gyroSensor_Data.xAxisValue = map(xAxisValue, -90, 90, 0, 254);
      gyroSensor_Data.yAxisValue = map(yAxisValue, -90, 90, 254, 0);

      vTaskDelay(100 / portTICK_PERIOD_MS);

      int reading = digitalRead(voice_button_pin);
      if (reading == LOW)
      {
        // Send Gyro Sensor data to VAIO (using ESP-NOW)
        uint8_t data[1 + sizeof(GyroSensor_Data)];
        data[0] = GYRO_SENSOR_DATA; // Header byte to identify the data type
        memcpy(data + 1, &gyroSensor_Data, sizeof(GyroSensor_Data));

        esp_err_t result =
            esp_now_send(broadcastAddress, (uint8_t *)&data, sizeof(data));
      }

      // Clear FIFO buffer
      mpu.resetFIFO();

#ifdef PRINT_DEBUG
      Serial.println(xAxisValue);
      Serial.println(yAxisValue);
#endif
    }
  }
}
