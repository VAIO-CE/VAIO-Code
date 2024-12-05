#include <Arduino.h>
#include <setup.h>

void setup()
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

//  Setup::Wifi();
//  Setup::ESPNOW();
//  Setup::WebServer();
//  Setup::Motors();
//  Setup::Ultrasonic();
//  Setup::Servo();
  Setup::DS4();
  Setup::InitialTask();

  vTaskDelete(NULL);
}

void loop()
{
  // Handled by FreeRTOS
}
