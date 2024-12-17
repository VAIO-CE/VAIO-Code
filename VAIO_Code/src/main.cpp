#include "setup.h"
#define CONFIG_ESPTOOLPY_FLASHFREQ CONFIG_ESPTOOLPY_FLASHFREQ_80M

void setup() {
  Serial.begin(115200);

  Setup::Wifi();
  Setup::ESPNOW();
  Setup::WebServer();
  Setup::LEDIndicators();
  Setup::Motors();
  Setup::Ultrasonic();
  Setup::Servo();
  Setup::DS4();
  Setup::InitialTask();

  vTaskDelete(NULL);
}

void loop() {}
