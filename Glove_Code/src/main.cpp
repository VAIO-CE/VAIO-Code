#include "constant.h"
#include "setup.h"

void setup()
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  Setup::ESPNOW();
  Setup::MPU6050();
  Setup::Buttons();
  Setup::SpeechRecognition();

  for (int i = 0; i < 10; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    Serial.println("Jalan");
  }

  Setup::InitialTask();
  vTaskDelete(NULL);
}

void loop() {}
