#include <WiFi.h>
#include <esp_now.h>

#define SIGNAL_TIMEOUT 500  // This is signal timeout in milli seconds.
#define STACK_SIZE 1024

struct PacketData
{
  byte xAxisValue;    
  byte yAxisValue;
} receiverData;

//Right motor
int enableRightMotor=5; 
int rightMotorPin1=2;
int rightMotorPin2=3;

//Left motor
int enableLeftMotor=6;
int leftMotorPin1=4;
int leftMotorPin2=7;

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  if (rightMotorSpeed < 0)
  {
    Serial.println("Right Motor Pin  1 : LOW");
    Serial.println("Right Motor Pin  2 : HIGH"); 
  }
  else if (rightMotorSpeed > 0)
  {
    Serial.println("Right Motor Pin  1 : HIGH");
    Serial.println("Right Motor Pin  2 : LOW");   
  }
  else
  {
    Serial.println("Right Motor Pin  1 : LOW");
    Serial.println("Right Motor Pin  2 : LOW");    
  }

  Serial.println();   
  
  if (leftMotorSpeed < 0)
  {
    Serial.println("Left Motor Pin  1 : LOW");
    Serial.println("Left Motor Pin  2 : HIGH");  
  }
  else if (leftMotorSpeed > 0)
  {
    Serial.println("Left Motor Pin  1 : HIGH");
    Serial.println("Left Motor Pin  2 : LOW");       
  }
  else
  {
    Serial.println("Right Motor Pin  1 : LOW");
    Serial.println("Right Motor Pin  2 : LOW");    
  }

  Serial.println();     

  // analogWrite(enableRightMotor, abs(rightMotorSpeed));
  // analogWrite(enableLeftMotor, abs(leftMotorSpeed));    
}

void vTaskHandGestureControl(void * pvParameters){
  int rightMotorSpeed=0;
  int leftMotorSpeed=0;

  int mappedYValue = map(receiverData.yAxisValue, 0, 254, -255, 255); 
  int mappedXValue = map(receiverData.xAxisValue, 0, 254, -255, 255); 
  int motorDirection = 1;

  if (mappedYValue < 0)
  {
    motorDirection = -1;
  }

  rightMotorSpeed = abs(mappedYValue) - mappedXValue;
  leftMotorSpeed = abs(mappedYValue) + mappedXValue;

  rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);
  leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);
  
  rotateMotor(rightMotorSpeed * motorDirection, leftMotorSpeed * motorDirection);

  vTaskDelete(NULL); // Delete this Task
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&receiverData, incomingData, sizeof(receiverData));

  // FreeRTOS Task
  xTaskCreatePinnedToCore(
    vTaskHandGestureControl,
    "Gesture Control",
    STACK_SIZE, // Stack
    NULL, // Parameter to pass function
    1, // Task Priority
    NULL, // Task Handle
    0 // CPU core
  );
}

void setupESPNOW(){
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  Serial.println("ESP-NOW initialized successfully");
}

void setup() {
  Serial.begin(115200);

  setupESPNOW();
}
 
void loop(){


}


