#include "setup.h"
#include <Arduino.h>

void setup()
{
  Serial.begin(115200);

  Setup::Wifi();
  Setup::ESPNOW();
  Setup::WebServer();
  Setup::LEDIndicators();
  Setup::Motors();
  Setup::Ultrasonic();
  Setup::Servo();
  // Setup::DS4();
  pinMode(vacuumPin, OUTPUT);
  digitalWrite(vacuumPin, HIGH);
  Setup::InitialTask();

  vTaskDelete(NULL);
}

void loop()
{
  // char *taskName = pcTaskGetTaskName(MasterControl::controlTaskHandle);
  // printf("Control Changed to : %s\n", taskName);
}
