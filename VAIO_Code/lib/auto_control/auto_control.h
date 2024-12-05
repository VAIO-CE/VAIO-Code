#ifndef AUTO_CONTROL_H
#define AUTO_CONTROL_H

#include "pin.h"
#include "constants.h"

#include "Arduino.h"
#include <ESP32Servo.h>


class AutoControl{
public:
    static Servo servo1;

    // Setting minimum duty cycle
    static int dutyCycle;
    static float distance;

    static void moveForward();
    static void moveBackward();
    static void moveInc(); 
    static void turnRight();
    static void turnLeft();
    static void moveStop();

    static int lookRight();
    static int lookLeft();
    static int readDistance();

    static void vTaskAutoControl(void * pvParameters);
};
#endif
