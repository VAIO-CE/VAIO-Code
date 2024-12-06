#include "sdkconfig.h"
#include "setup.h"
#include <Arduino.h>
#include <esp_log.h>

void setup()
{
  Serial.begin(115200);
  esp_log_level_set("ps4_L2CAP", ESP_LOG_NONE);
  Setup::Wifi();
  Setup::ESPNOW();
  Setup::WebServer();
  Setup::LEDIndicators();
  Setup::Motors();
//  Setup::Ultrasonic();
//  Setup::Servo();
 // Setup::DS4();
  Setup::InitialTask();

  vTaskDelete(NULL);
}

void loop()
{
  // Handled by FreeRTOS
}
