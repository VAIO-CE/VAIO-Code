#ifndef AUTO_CONTROL_H
#define AUTO_CONTROL_H

#include "pin.h"
#include "constants.h"

#include <Arduino.h>
#include <freertos/task.h>
#include <ESP32Servo.h>

class AutoControl{
private:
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
public:
    static Servo servo;
    static void vTaskAutoControl(void * pvParameters);
};
#endif
