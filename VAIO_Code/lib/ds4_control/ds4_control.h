#ifndef DS4_CONTROL_H
#define DS4_CONTROL_H

#include "pin.h"
#include "constants.h"
#include "motor.h"

#include <Preferences.h>
#include <ps4Controller.h>
#include <master_control.h>

class DS4Control
{
public:
    static ps4Controller ps4;
    static Preferences preferences;

    static void initializePreferences();
    static void vTaskDS4Setup(void *pvParameters);
    static void vTaskDS4Control(void *pvParameters);
    static void onConnect();
    static void onDisconnect();
    static void rotateMotorFromDS4(int rightMotorSpeed, int leftMotorSpeed);
};

#endif
