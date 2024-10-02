#ifndef VR3SENSOR_H
#define VR3SENSOR_H

#include <pin.h>
#include <constant.h>
#include <data_types.h>
#include <esp_now.h>
#include "VoiceRecognitionV3.h"



class VR3Sensor{
    public:
        static VR vr3;
        static struct VR3Sensor_Data vr3_sensor_data;
        static uint8_t vr3_buf[50];

        static void vTaskVoiceRecognition(void *pvParameters);
        static boolean byteArrayCmp(uint8_t *a, uint8_t *b, int len_a, int len_b);
        static boolean checkForVR3LoadResponse(int timeout);   

};
#endif