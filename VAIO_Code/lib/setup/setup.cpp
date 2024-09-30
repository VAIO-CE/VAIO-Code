#include <setup.h>

void Setup::Wifi(){
  // Set device as a Wi-Fi AP Station
  WiFi.mode(WIFI_AP_STA);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  Serial.println("WiFi started");

}

void Setup::WebServer(){

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }

  WebServer::WebListener();
  WebServer::server.begin();

  Serial.println("HTTP server started");
}

void Setup::ESPNOW(){

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  Serial.println("ESP-NOW started");
  
  // Receive data from ESP-NOW
  esp_now_register_recv_cb(esp_now_recv_cb_t(MasterControl::ESPNOW_OnDataReceive));
}

void Setup::Motors(){
  // sets the pins as outputs:
  pinMode(motorRightPin1, OUTPUT);
  pinMode(motorRightPin2, OUTPUT);
  pinMode(motorLeftPin1, OUTPUT);
  pinMode(motorLeftPin2, OUTPUT);

  Serial.println("Motor Pins Initialized");
}

void Setup::Ultrasonic(){
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.println("Ultrasonic Pins Initialized");
}

void Setup::Servo(){
  AutoControl::servo1.attach(servoPin);
  AutoControl::servo1.write(115);
  Serial.println("Servo Pins Initialized");
}

void Setup::InitialTask(){
  xTaskCreatePinnedToCore(AutoControl::vTaskAutoControl, "Automatic Control", STACK_SIZE, NULL, 1, &MasterControl::controlTaskHandle, 0);
  //xTaskCreatePinnedToCore(GyroControl::vTaskGestureControl, "Gyro Control", STACK_SIZE, NULL, 1, &MasterControl::controlTaskHandle, 0);
  Serial.println("Auto Control Initialized");
}