#include <setup.h>

void Setup::ESPNOW(){
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(GyroControl::OnDataReceive));
  Serial.println("ESP-NOW initialized successfully");
}

void Setup::Motors(){
  // sets the pins as outputs:
  pinMode(motorRightPin1, OUTPUT);
  pinMode(motorRightPin2, OUTPUT);
  pinMode(enableRightPin, OUTPUT);
  pinMode(motorLeftPin1, OUTPUT);
  pinMode(motorLeftPin2, OUTPUT);
  pinMode(enableLeftPin, OUTPUT);
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