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

     void moveForward();
     void moveBackward();
     void moveInc(); 
     void turnRight();
     void turnLeft();
     void moveStop();

     int lookRight();
     int lookLeft();
     int readDistance();
public:
     static inline Servo servo {};
     static void vTaskAutoControl(void * pvParameters);
};
#endif
