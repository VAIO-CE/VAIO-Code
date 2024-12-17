#include "ds4_control.h"

void DS4Control::vTaskDS4Setup(void *pvParameters)
{
  preferences.begin(MAC_ADDR_STORAGE_NAMESPACE, false);

  const char *btmac =
      DS4Control::preferences.getString("btmac", "D0:27:88:51:4C:50").c_str();

  while (btmac == "" || btmac == "empty")
  {
    Serial.println("MAC address fetch error!");
    delay(3000);
  }

  DS4Control::preferences.end();

  DS4Control::ps4.attachOnConnect(DS4Control::onConnect);
  DS4Control::ps4.attachOnDisconnect(DS4Control::onDisconnect);

  // Connect
  DS4Control::ps4.begin(btmac);

  while (!DS4Control::ps4.isConnected())
  {
    Serial.println("PS4 Controller Not Found!");
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }

  xTaskCreatePinnedToCore(vTaskDS4Control, "DS4 Control", 2 * STACK_SIZE, NULL, 1, &(MasterControl::controlTaskHandle), 0);

  vTaskDelete(NULL);
}

void DS4Control::vTaskDS4Control(void *pvParamaters)
{
  int threshold = 30;
  while (true)
  {
    digitalWrite(LED_BUILTIN, LOW);

    int yAxisValue{(ps4.data.analog.stick.ly)};
    int xAxisValue{(ps4.data.analog.stick.rx)};

    int throttle{map(yAxisValue, 127, -127, -255, 255)};
    int steering{map(xAxisValue, -127, 127, -255, 255)};

    int motorDirection = 1;

    if (throttle < -threshold)
    {
      motorDirection = -1;
    }

    throttle = abs(throttle) < threshold ? 0 : throttle;
    steering = abs(steering) < threshold ? 0 : steering;

    int rightMotorSpeed{abs(throttle) - steering};
    int leftMotorSpeed{abs(throttle) + steering};

    rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);
    leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);

    Motor::rotateMotor(rightMotorSpeed * motorDirection, leftMotorSpeed * motorDirection);

    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void DS4Control::onConnect() {}

void DS4Control::onDisconnect()
{
  Motor::rotateMotor(0, 0);
}

