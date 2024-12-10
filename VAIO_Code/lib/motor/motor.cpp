#include "motor.h"
#include "constants.h"

void Motor::rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  // Serial.println("Motor rotated");

  int leftSpeed = map(abs(leftMotorSpeed), 0, 255, MIN_SPEED_MOTOR, MAX_SPEED_MOTOR);
  int rightSpeed = map(abs(rightMotorSpeed), 0, 255, MIN_SPEED_MOTOR, MAX_SPEED_MOTOR);

  ledcWrite(PWM_Channel_Left, leftSpeed);
  ledcWrite(PWM_Channel_Right, rightSpeed);

  if (rightMotorSpeed < 0)
  {
    digitalWrite(motorRightPin1, LOW);
    digitalWrite(motorRightPin2, HIGH);
  }
  else if (rightMotorSpeed > 0)
  {
    digitalWrite(motorRightPin1, HIGH);
    digitalWrite(motorRightPin2, LOW);
  }
  else
  {
    digitalWrite(motorRightPin1, LOW);
    digitalWrite(motorRightPin2, LOW);
  }

  if (leftMotorSpeed < 0)
  {
    digitalWrite(motorLeftPin1, LOW);
    digitalWrite(motorLeftPin2, HIGH);
  }
  else if (leftMotorSpeed > 0)
  {
    digitalWrite(motorLeftPin1, HIGH);
    digitalWrite(motorLeftPin2, LOW);
  }
  else
  {
    digitalWrite(motorLeftPin1, LOW);
    digitalWrite(motorLeftPin2, LOW);
  }
}
