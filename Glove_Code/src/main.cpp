#include <setup.h>
#include <button_sensor.h>

void setup() {

  Serial.begin(115200);
  pinMode(buttonPin, INPUT);

  Setup::ESPNOW();
  Setup::MPU6050();

  xTaskCreatePinnedToCore(
    GyroSensor::vTaskGestureControl,
    "Hand Gesture Control",
    STACK_SIZE, // Stack
    NULL, // Parameter to pass function
    1, // Task Priority
    NULL, // Task Handle
    0 // CPU core
  );

  xTaskCreatePinnedToCore(
    ButtonSensor::vTaskButtonControl,
    "Button Control",
    STACK_SIZE, // Stack
    NULL, // Parameter to pass function
    1, // Task Priority
    NULL, // Task Handle
    1 // CPU core
  );
  
}
 
void loop() {
  
}