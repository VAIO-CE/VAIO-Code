#include "setup.h"
#include "Arduino.h"
#include "master_control.h"

esp_now_peer_info_t Setup::peerInfo;

void Setup::Wifi() {
  // Set device as a Wi-Fi AP Station
  WiFi.mode(WIFI_AP_STA);

  WiFi.softAP(AP_SSID, AP_PASS, 1, 0, 1, false);
  WiFi.softAPConfig(LOCAL_IP, GATEWAY, SUBNET);
  vTaskDelay(100 / portTICK_PERIOD_MS);
  Serial.println("WiFi started");

  Serial.print("[DEFAULT] ESP32 Board MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void Setup::WebServer() {

  /*use MDNS for host name resolution*/
  // http://vaio.local/
  if (!MDNS.begin(HOST)) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }

  Serial.println("mDNS started!");

  WebServer::WebListener();
  WebServer::server.begin();

  Serial.println("API server running. Connect at http://vaio.local/");
  vTaskDelay(500 / portTICK_PERIOD_MS);
}

void Setup::ESPNOW() {
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(MasterControl::ESPNOW_OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, GloveAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  // Receive data from ESP-NOW
  esp_now_register_recv_cb(
      esp_now_recv_cb_t(MasterControl::ESPNOW_OnDataReceive));
}

void Setup::LEDIndicators() {

  // sets the pins as outputs:
  pinMode(autoLEDPin, OUTPUT);
  pinMode(gyroLEDPin, OUTPUT);
  pinMode(ds4LEDPin, OUTPUT);

  Serial.println("LED Pins Initialized");
}

void Setup::Motors() {

  // Center Gyro data at initial
  GyroControl::gyroSensor_Data.xAxisValue = 127;
  GyroControl::gyroSensor_Data.yAxisValue = 127;

  // configure PWM
  ledcSetup(PWM_Channel_Left, PWM_Frequency, PWM_Resolution);
  ledcAttachPin(motorPWMLeftPin, PWM_Channel_Left);

  ledcSetup(PWM_Channel_Right, PWM_Frequency, PWM_Resolution);
  ledcAttachPin(motorPWMRightPin, PWM_Channel_Right);

  // sets the pins as outputs:
  pinMode(motorRightPin1, OUTPUT);
  pinMode(motorRightPin2, OUTPUT);
  pinMode(motorLeftPin1, OUTPUT);
  pinMode(motorLeftPin2, OUTPUT);

  Serial.println("Motor Pins Initialized");
}

void Setup::Ultrasonic() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input
  Serial.println("Ultrasonic Pins Initialized");
}

void Setup::Servo() {
  AutoControl::servo.attach(servoPin);
  AutoControl::servo.write(115);
  Serial.println("Servo Pins Initialized");
}

void Setup::DS4() {
  xTaskCreatePinnedToCore(DS4Control::vTaskDS4Setup, "DS4 Task Setup",
                          STACK_SIZE * 2, NULL, 1, NULL, 1);
}

void Setup::InitialTask() {

  // Send Initial Control State to Glove Code
  MasterControl::controlData[0] =
      ControlState::GYRO_CONTROL; // Change this depending on the inital state
  esp_now_send(GloveAddress, (uint8_t *)&MasterControl::controlData, sizeof(MasterControl::controlData));

  MasterControl::changeLEDIndicator(ControlState::GYRO_CONTROL);

  // xTaskCreatePinnedToCore(AutoControl::vTaskAutoControl, "Automatic
  // Control", STACK_SIZE, NULL, 1, &MasterControl::controlTaskHandle, 0);
  xTaskCreatePinnedToCore(GyroControl::vTaskGestureControl, "Gyro Control",
                          STACK_SIZE, NULL, 1,
                          &MasterControl::controlTaskHandle, 0);
  // xTaskCreatePinnedToCore(DS4Control::vTaskDS4Control, "DS4 Control", 2 *
  // STACK_SIZE, NULL, 1, &MasterControl::controlTaskHandle, 0);
  // Send Gyro Sensor data to VAIO (using ESP-NOW)

  Serial.println("Task Initialized");
}
