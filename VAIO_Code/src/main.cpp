#include "setup.h"
#include <Arduino.h>

void setup() {
  Serial.begin(115200);

  Setup::Wifi();
  Setup::ESPNOW();
  Setup::WebServer();
  Setup::LEDIndicators();
  Setup::Motors();
  //  Setup::Ultrasonic();
  //  Setup::Servo();
  Setup::DS4();
  Setup::InitialTask();

  vTaskDelete(NULL);
}

void loop() {}
