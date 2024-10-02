#include <vr3_sensor.h>

struct VR3Sensor_Data VR3Sensor::vr3_sensor_data;
uint8_t VR3Sensor::vr3_buf[50];

boolean VR3Sensor::checkForVR3LoadResponse(int timeout){
  int ret_len;

  ret_len = vr3.receive_pkt(vr3_buf, timeout);
  Serial.println(ret_len);
  if (ret_len <= 0)
    return false;

  return byteArrayCmp(vr3_buf, vr3_load_response, ret_len, sizeof(vr3_load_response));
}


boolean VR3Sensor::byteArrayCmp(uint8_t *a, uint8_t *b, int len_a, int len_b){
  if (len_a != len_b)
    return false;

  for (int n = 0; n < len_a; n++)
    if (a[n] != b[n])
      return false;

  return true;
}

void VR3Sensor::vTaskVoiceRecognition(void *pvParameters){
  Serial.println("Voice recognition task started.");
  
  int ret_len;

  while (1) {
    ret_len = vr3.receive_pkt(vr3_buf, 50);
    if (ret_len > 0) {
      if (byteArrayCmp(vr3_buf, vr3_move1_msg, ret_len, sizeof(vr3_move1_msg)) ||
          byteArrayCmp(vr3_buf, vr3_move2_msg, ret_len, sizeof(vr3_move2_msg))) {
        
        #ifdef PRINT_DEBUG  
          Serial.println(F("Heard: move  "));
        #endif
        vr3_sensor_data.command = 0x01;
        
      }
      else if (byteArrayCmp(vr3_buf, vr3_control1_msg, ret_len, sizeof(vr3_control1_msg)) ||
                byteArrayCmp(vr3_buf, vr3_control2_msg, ret_len, sizeof(vr3_control2_msg)) || 
                byteArrayCmp(vr3_buf, vr3_control3_msg, ret_len, sizeof(vr3_control3_msg))) {
        #ifdef PRINT_DEBUG  
          Serial.println(F("Heard: control"));
        #endif        
        vr3_sensor_data.command = 0x02;
      }
      else if (byteArrayCmp(vr3_buf, vr3_hand1_msg, ret_len, sizeof(vr3_hand1_msg)) ||
                byteArrayCmp(vr3_buf, vr3_hand2_msg, ret_len, sizeof(vr3_hand2_msg))) {
        #ifdef PRINT_DEBUG  
          Serial.println(F("Heard: hand "));
        #endif
        vr3_sensor_data.command = 0x03;
      }
      else if (byteArrayCmp(vr3_buf, vr3_test_msg, ret_len, sizeof(vr3_test_msg))) {      
        Serial.println(F("Heard: test "));
        // Other tasks to do
      }
      else {
        Serial.println(F("Unknown word"));
        // Other tasks to do
      }

      vTaskDelay(100 / portTICK_PERIOD_MS);

      uint8_t data[1 + sizeof(VR3Sensor_Data)];
      data[0] = VR3_SENSOR_DATA;
      memcpy(data + 1, &vr3_sensor_data, sizeof(VR3Sensor_Data));

      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &data, sizeof(data));

    }

    vTaskDelay(100 / portTICK_PERIOD_MS); 
  }
}