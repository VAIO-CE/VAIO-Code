#include "vacuum.h"
uint8_t Vacuum::buttonToggleData = 0x01;

void Vacuum::transmitVacuumData() {
  // Prepare the data to be sent
  buttonToggleData = (buttonToggleData == 0x01) ? 0x02 : 0x01;

  uint8_t data[1 + sizeof(buttonToggleData)];
  data[0] = VACUUM_DATA;
  memcpy(data + 1, &buttonToggleData, sizeof(buttonToggleData));

  // Send the data using ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, data, sizeof(data));
  if (result != ESP_OK) {
    Serial.println("Error sending button data");
  }
}

void Vacuum::vTaskVacuumControl(void *pvParameters) {

  int buttonState;           // the current reading from the input pin
  int lastButtonState = LOW; // the previous reading from the input pin

  unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 50; // 50ms debounce time

  while (1) {
    // Read the button state
    int reading = digitalRead(vacuum_button_pin);
    if (reading != lastButtonState) {
      // reset the debouncing timer
      lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (reading != buttonState) {
        buttonState = reading;
        if (buttonState == HIGH)
          transmitVacuumData();
      }
    }

    lastButtonState = reading;
  }
}
