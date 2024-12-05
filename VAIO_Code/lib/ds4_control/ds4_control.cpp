#include <ds4_control.h>

ps4Controller DS4Control::ps4;
Preferences DS4Control::preferences;

void DS4Control::initializePreferences()
{
    preferences.begin(MAC_ADDR_STORAGE_NAMESPACE, false);
}

void DS4Control::vTaskDS4Setup(void * pvParameters)
{
  DS4Control::initializePreferences();

  const char *btmac = DS4Control::preferences.getString("btmac", "D0:27:88:51:4C:50").c_str();

  while (btmac == "" || btmac == "empty")
  {
    Serial.println("MAC address fetch error!");
    delay(3000);
  }

  DS4Control::preferences.end();

  // Connect
  DS4Control::ps4.begin(btmac);

  while (!DS4Control::ps4.isConnected())
  {
    Serial.println("PS4 Controller Not Found!");
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }

  Serial.println("Ready & Connected!");
  vTaskDelete(NULL);
}


void DS4Control::vTaskDS4Control(void *pvParamaters)
{
    int threshold = 20;
    while (true)
    {
        digitalWrite(LED_BUILTIN, LOW);

        int yAxisValue{-(ps4.data.analog.stick.ly)};
        int xAxisValue{(ps4.data.analog.stick.rx)};

        int throttle{map(yAxisValue, -127, 127, -255, 255)};
        int steering{map(xAxisValue, -127, 127, -255, 255)};
        int motorDirection = 1;

        if (throttle < -threshold)
        {
            motorDirection = -1;
        }

        int rightMotorSpeed{abs(throttle) - steering};
        int leftMotorSpeed{abs(throttle) + steering};

        rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);
        leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);

        rotateMotor(rightMotorSpeed * motorDirection, leftMotorSpeed * motorDirection);

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void DS4Control::onConnect()
{
    Serial.println("DS4 Connected!");
}

void DS4Control::onDisconnect()
{
    rotateMotor(0, 0);
    Serial.println("DS4 Disconnected!");
}

void DS4Control::rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
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
