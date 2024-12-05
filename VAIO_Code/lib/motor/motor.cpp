#include "motor.h"

void Motor::rotateMotor(int rightMotorSpeed, int leftMotorSpeed) {
     ledcWrite(motorPWMLeft, abs(leftMotorSpeed));
     ledcWrite(motorPWMRight, abs(rightMotorSpeed));

     // Control Right Motor Direction
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
