#include <Arduino.h>
#include <setup.h>

void setup()
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  Setup::Wifi();
  Setup::ESPNOW();
  Setup::WebServer();
  Setup::Motors();
  Setup::Ultrasonic();
  Setup::Servo();
<<<<<<< HEAD
  // Setup::DS4();
=======
  //  Setup::DS4();
>>>>>>> 59f73823e62ad685390be2bdfe22eb1dd0d8346e
  Setup::InitialTask();

  vTaskDelete(NULL);
}

void loop()
{
  // Handled by FreeRTOS
}