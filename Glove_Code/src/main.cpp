#include "constant.h"
#include "setup.h"

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  Setup::ESPNOW();
  Setup::MPU6050();
  Setup::Buttons();
  Setup::SpeechRecognition();
  Setup::InitialTask();
  vTaskDelete(NULL);
}

void loop() {}
