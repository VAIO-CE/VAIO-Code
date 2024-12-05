#include <setup.h>

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

  Serial.println("ESP-NOW started");

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

  // configure PWM
  ledcSetup(motorPWMLeft, PWM_Frequency, PWM_Resolution);
  ledcAttachPin(motorPWMLeft, motorPWMLeft);

  ledcSetup(motorPWMRight, PWM_Frequency, PWM_Resolution);
  ledcAttachPin(motorPWMRight, motorPWMRight);

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

void testTask(void *parameter) {
  while (1) {
    Serial.println("Testing");
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void Setup::InitialTask() {
  // xTaskCreatePinnedToCore(AutoControl::vTaskAutoControl, "Automatic Control",
  // STACK_SIZE, NULL, 1, &MasterControl::controlTaskHandle, 0);
  // xTaskCreatePinnedToCore(GyroControl::vTaskGestureControl, "Gyro Control",
  // STACK_SIZE, NULL, 1, &MasterControl::controlTaskHandle, 0);
  Serial.println("Auto Control Initialized");
  xTaskCreatePinnedToCore(testTask, "DS4 Control", 2 * STACK_SIZE, NULL, 1,
                          NULL, 0);
  // xTaskCreatePinnedToCore(DS4Control::vTaskDS4Control, "DS4 Control", 2 *
  // STACK_SIZE, NULL, 1, &MasterControl::controlTaskHandle, 0);
}
