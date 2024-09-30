#include <setup.h>

void setup() {
  Serial.begin(115200);
  
  Setup::ESPNOW();
  Setup::MPU6050();
  Setup::InitialTask();
  
}
 
void loop() {
  
}