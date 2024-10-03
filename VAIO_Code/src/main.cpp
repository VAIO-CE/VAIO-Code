
#include <Arduino.h>
#include <setup.h>
#include <ds4_control.h>

void setup()
{
  Serial.begin(115200);

  Setup::Wifi();
  Setup::ESPNOW();
  Setup::WebServer();
  Setup::Motors();
  Setup::Ultrasonic();
  Setup::Servo();
  Setup::DS4();
  Setup::InitialTask();

  vTaskDelete(NULL);
}

void loop()
{
}