#include "constants.h"
#include "esp32-hal-ledc.h"
#include "pin.h"
#include <auto_control.h>

Servo AutoControl::servo;
int AutoControl::dutyCycle = 60;
float AutoControl::distance = 100.0f;

void AutoControl::moveForward() {
  // Move the DC motor forward at maximum speed
  digitalWrite(motorRightPin1, HIGH);
  digitalWrite(motorRightPin2, LOW);
  digitalWrite(motorLeftPin1, HIGH);
  digitalWrite(motorLeftPin2, LOW);
}
void AutoControl::moveBackward() {
  // Move DC motor backwards at maximum speed
  digitalWrite(motorRightPin1, LOW);
  digitalWrite(motorRightPin2, HIGH);
  digitalWrite(motorLeftPin1, LOW);
  digitalWrite(motorLeftPin2, HIGH);
}
void AutoControl::moveInc() {
  // Move DC motor forward with increasing speed
  digitalWrite(motorRightPin1, HIGH);
  digitalWrite(motorRightPin2, LOW);
  digitalWrite(motorLeftPin1, HIGH);
  digitalWrite(motorLeftPin2, LOW);
  while (dutyCycle <= 255) {
    ledcWrite(PWM_Channel_Right, dutyCycle);
    dutyCycle = dutyCycle + 5;
    vTaskDelay(500 / portTICK_RATE_MS);
    delay(500);
  }
  dutyCycle = 60;
}
void AutoControl::turnRight() {
  digitalWrite(motorRightPin1, LOW);
  digitalWrite(motorRightPin2, HIGH);
  digitalWrite(motorLeftPin1, HIGH);
  digitalWrite(motorLeftPin2, LOW);

  vTaskDelay(1500 / portTICK_RATE_MS);

  digitalWrite(motorRightPin1, HIGH);
  digitalWrite(motorRightPin2, LOW);
  digitalWrite(motorLeftPin1, HIGH);
  digitalWrite(motorLeftPin2, LOW);
}
void AutoControl::turnLeft() {
  digitalWrite(motorRightPin1, HIGH);
  digitalWrite(motorRightPin2, LOW);
  digitalWrite(motorLeftPin1, LOW);
  digitalWrite(motorLeftPin2, HIGH);

  vTaskDelay(1500 / portTICK_RATE_MS);

  digitalWrite(motorRightPin1, HIGH);
  digitalWrite(motorRightPin2, LOW);
  digitalWrite(motorLeftPin1, HIGH);
  digitalWrite(motorLeftPin2, LOW);
}
void AutoControl::moveStop() {
  // Stop the DC motor
  digitalWrite(motorRightPin1, LOW);
  digitalWrite(motorRightPin2, LOW);
  digitalWrite(motorLeftPin1, LOW);
  digitalWrite(motorLeftPin2, LOW);
}

int AutoControl::lookRight() {
  servo.write(40);
  vTaskDelay(500 / portTICK_RATE_MS);
  int distance = readDistance();
  vTaskDelay(100 / portTICK_RATE_MS);
  servo.write(115);
  return distance;
}
int AutoControl::lookLeft() {
  servo.write(190);
  vTaskDelay(500 / portTICK_RATE_MS);
  servo.write(115);
  int distance = readDistance();
  vTaskDelay(100 / portTICK_RATE_MS);
  servo.write(115);
  return distance;
}
int AutoControl::readDistance() {
  unsigned long duration;
  float distanceCm;

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_SPEED / 2;

  return distanceCm;
}

void AutoControl::vTaskAutoControl(void *pvParameters) {
  while (true) {
    int distanceR = 0;
    int distanceL = 0;
    vTaskDelay(40 / portTICK_RATE_MS);
    if (distance <= 15) {
      moveStop();
      vTaskDelay(100 / portTICK_RATE_MS);
      moveBackward();
      vTaskDelay(300 / portTICK_RATE_MS);
      moveStop();
      vTaskDelay(200 / portTICK_RATE_MS);
      distanceR = lookRight();
      vTaskDelay(200 / portTICK_RATE_MS);
      vTaskDelay(200 / portTICK_RATE_MS);
      distanceL = lookLeft();

      if (distanceR >= distanceL) {
        turnRight();
        moveStop();
      } else {
        turnLeft();
        moveStop();
      }
    } else {
      moveForward();
    }
    distance = readDistance();
  }
}
