#include <button_sensor.h>

// uint8_t ButtonSensor::buttonToggleData = 0x01;

void ButtonSensor::transmitButtonData() {
  // Prepare the data to be sent
  // buttonToggleData = (buttonToggleData == 0x01) ? 0x02 : 0x01;
  
  // uint8_t data[1 + sizeof(buttonToggleData)];
  // data[0] = BUTTON_DATA; // Header byte to identify the data type
  // memcpy(data + 1, &buttonToggleData, sizeof(buttonToggleData));

  // // Send the data using ESP-NOW
  // esp_err_t result = esp_now_send(broadcastAddress, data, sizeof(data));
  // if (result != ESP_OK) {
  //   Serial.println("Error sending button data");
  // }
}


void ButtonSensor::vTaskButtonControl(void * pvParameters){
  // int buttonState;            // the current reading from the input pin
  // int lastButtonState = LOW;  // the previous reading from the input pin

  // unsigned long lastDebounceTime = 0;
  // const unsigned long debounceDelay = 50; // 50ms debounce time

  // while(1){
  //   // Read the button state
  //   int reading = digitalRead(buttonPin); // Replace with the correct GPIO pin
    
  //   if (reading != lastButtonState) {
  //     // reset the debouncing timer
  //     lastDebounceTime = millis();
  //   }

  //   if((millis() - lastDebounceTime) > debounceDelay){
  //     if(reading != buttonState){
  //       buttonState = reading;
  //       if (buttonState == HIGH) transmitButtonData();
  //     }
  //   }

  //   lastButtonState = reading;

  // }
}