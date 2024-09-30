#include <setup.h>

void Setup::ESPNOW(){
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    return;
  }
  
  // Receive data from ESP-NOW
  esp_now_register_recv_cb(esp_now_recv_cb_t(MasterControl::ESPNOW_OnDataReceive));

}

void Setup::Motors(){
  // sets the pins as outputs:
  pinMode(motorRightPin1, OUTPUT);
  pinMode(motorRightPin2, OUTPUT);
  pinMode(motorLeftPin1, OUTPUT);
  pinMode(motorLeftPin2, OUTPUT);
}

void Setup::Ultrasonic(){
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}
void Setup::Servo(){
  AutoControl::servo1.attach(servoPin);
  AutoControl::servo1.write(115);
  delay(2000);
  AutoControl::distance = AutoControl::readDistance();
  delay(100);
  AutoControl::distance = AutoControl::readDistance();
  delay(100);
  AutoControl::distance = AutoControl::readDistance();
  delay(100);
  AutoControl::distance = AutoControl::readDistance();
  delay(100);
}


void Setup::InitialControl(){
  xTaskCreatePinnedToCore(GyroControl::vTaskGestureControl, "Gyro Control", STACK_SIZE, NULL, 1, &MasterControl::controlTaskHandle, 0);
}