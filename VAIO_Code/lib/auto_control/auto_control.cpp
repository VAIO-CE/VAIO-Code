#include <auto_control.h>

Servo AutoControl::servo1;
int AutoControl::dutyCycle = 60;
float AutoControl::distance = 100.0f;

void AutoControl::moveForward() {
    // Move the DC motor forward at maximum speed
  digitalWrite(motorRightPin1, HIGH);
  digitalWrite(motorRightPin2, LOW);
  digitalWrite(motorLeftPin1, HIGH);
  digitalWrite(motorLeftPin2, LOW); 
}
void AutoControl::moveBackward(){
    // Move DC motor backwards at maximum speed
  digitalWrite(motorRightPin1, LOW);
  digitalWrite(motorRightPin2, HIGH);
  digitalWrite(motorLeftPin1, LOW);
  digitalWrite(motorLeftPin2, HIGH); 
}
void AutoControl::moveInc(){
  // Move DC motor forward with increasing speed
  digitalWrite(motorRightPin1, HIGH);
  digitalWrite(motorRightPin2, LOW);
  digitalWrite(motorLeftPin1, HIGH);
  digitalWrite(motorLeftPin2, LOW); 
  while (dutyCycle <= 255){
    dutyCycle = dutyCycle + 5;
    delay(500);
  }
  dutyCycle = 60;

}
void AutoControl::turnRight(){
    digitalWrite(motorRightPin1, LOW);
  digitalWrite(motorRightPin2, HIGH);
  digitalWrite(motorLeftPin1, HIGH);
  digitalWrite(motorLeftPin2, LOW); 

  // motor1.run(FORWARD);
  // motor2.run(FORWARD);
  // motor3.run(BACKWARD);
  // motor4.run(BACKWARD); 
    
  delay(1500);
  // motor1.run(FORWARD);      
  // motor2.run(FORWARD);
  // motor3.run(FORWARD);
  // motor4.run(FORWARD);
  digitalWrite(motorRightPin1, HIGH);
  digitalWrite(motorRightPin2, LOW);
  digitalWrite(motorLeftPin1, HIGH);
  digitalWrite(motorLeftPin2, LOW); 
 
}
void AutoControl::turnLeft(){
    // motor1.run(BACKWARD);     
  // motor2.run(BACKWARD);  
  // motor3.run(FORWARD);
  // motor4.run(FORWARD);
  digitalWrite(motorRightPin1, HIGH);
  digitalWrite(motorRightPin2, LOW);
  digitalWrite(motorLeftPin1, LOW);
  digitalWrite(motorLeftPin2, HIGH); 
 
  delay(1500);
  // motor1.run(FORWARD);     
  // motor2.run(FORWARD);
  // motor3.run(FORWARD);
  // motor4.run(FORWARD);
  digitalWrite(motorRightPin1, HIGH);
  digitalWrite(motorRightPin2, LOW);
  digitalWrite(motorLeftPin1, HIGH);
  digitalWrite(motorLeftPin2, LOW);

}
void AutoControl::moveStop(){
    // Stop the DC motor
  digitalWrite(motorRightPin1, LOW);
  digitalWrite(motorRightPin2, LOW);
  digitalWrite(motorLeftPin1, LOW);
  digitalWrite(motorLeftPin2, LOW); 
}

int AutoControl::lookRight(){
    servo1.write(40); 
    delay(500);
    int distance = readDistance();
    delay(100);
    servo1.write(115); 
    return distance;
}
int AutoControl::lookLeft(){
    servo1.write(190); 
    delay(500);
    int distance = readDistance();
    delay(100);
    servo1.write(115); 
    return distance;
}
int AutoControl::readDistance(){
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
  distanceCm = duration * SOUND_SPEED/2;

  return distanceCm;
}

void AutoControl::vTaskAutoControl(void * pvParameters){
  while(true){
    int distanceR = 0;
    int distanceL =  0;
    delay(40);

    if(distance<=15)
    {
    moveStop();
      delay(100);
      moveBackward();
      delay(300);
      moveStop();
      delay(200);
      distanceR = lookRight();
      delay(200);
      distanceL = lookLeft();
      delay(200);

      if(distanceR>=distanceL)
      {
        turnRight();
        moveStop();
      }else
      {
        turnLeft();
        moveStop();
      }
    }else
    {
      moveForward();
    }
    distance = readDistance();
  }
}