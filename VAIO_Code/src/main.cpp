
#include <Arduino.h>
#include <setup.h>

void setup() {
  Serial.begin(115200);

  Setup::Wifi();
  Setup::ESPNOW();
  Setup::WebServer();
  Setup::Motors();
  Setup::Ultrasonic();
  Setup::Servo();
  Setup::InitialTask();
}

void loop(){
  

}