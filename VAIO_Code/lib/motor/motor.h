#ifndef MOTOR_H
#define MOTOR_H

#include "pin.h"
#include <Arduino.h>

class Motor {
public:
  static void rotateMotor(int rightMotorSpeed, int leftMotorSpeed);
};
#endif
