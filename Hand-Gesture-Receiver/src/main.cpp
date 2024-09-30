
#include <Arduino.h>
#include <setup.h>

void setup() {
  Serial.begin(115200);

  Setup::ESPNOW();
  Setup::Motors();
  Setup::Ultrasonic();
  Setup::Servo();

  xTaskCreatePinnedToCore(
    AutoControl::vTaskAutoControl,
    "Auto Control",
    STACK_SIZE, // Stack
    NULL, // Parameter to pass function
    0, // Task Priority
    NULL, // Task Handle
    0 // CPU core
  );
}

void loop(){
  

}