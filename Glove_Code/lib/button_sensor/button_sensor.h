#ifndef BUTTONSENSOR_H
#define BUTTONSENSOR_H

#include <Arduino.h>
#include <esp_now.h>
#include <data_types.h>
#include <constant.h>
#include <pin.h>

class ButtonSensor{
    public:
        static uint8_t buttonToggleData;
        static void transmitButtonData();
        static void vTaskButtonControl(void * pvParameters);
};
#endif