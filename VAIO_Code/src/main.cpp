
#include <Arduino.h>
#include <setup.h>

void setup() {
  Serial.begin(115200);

  Setup::ESPNOW();
  Setup::Motors();
  Setup::Ultrasonic();
  Setup::Servo();
  Setup::InitialControl();
}

void loop(){
  

}