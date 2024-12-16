#ifndef AUTO_CONTROL_H
#define AUTO_CONTROL_H

#include "pin.h"
#include "constants.h"

#include <Arduino.h>
#include <freertos/task.h>
#include <ESP32Servo.h>

class AutoControl{
private:
    static constexpr int dutyCycle {MIN_SPEED_MOTOR+ 25};
    static inline float distance {100.0f};

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
    static inline Servo servo {};
    static void vTaskAutoControl(void * pvParameters);
};
#endif
