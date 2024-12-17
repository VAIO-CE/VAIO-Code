#ifndef DS4_CONTROL_H
#define DS4_CONTROL_H

#include "constants.h"
#include "motor.h"
#include "pin.h"

#include <Preferences.h>
#include <master_control.h>
#include <ps4Controller.h>

class DS4Control {
private:
  static inline ps4Controller ps4{};
  static inline Preferences preferences{};

  static void onConnect();
  static void onDisconnect();

public:
  static void vTaskDS4Setup(void *pvParameters);
  static void vTaskDS4Control(void *pvParameters);
};

#endif
