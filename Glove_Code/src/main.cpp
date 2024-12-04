#include <setup.h>

void setup() {
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);
  Setup::ESPNOW();
  Setup::MPU6050();
  Setup::SpeechRecognition();
  Setup::InitialTask();
  
}
 
void loop() {
  
}