#ifndef CONSTANT_H
#define CONSTANT_H

// This is signal timeout in milli seconds.
#define SIGNAL_TIMEOUT 500

// define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

// Setting PWM properties
#define PWM_Frequency 30000
#define PWM_Resolution  8

// FreeRTOS Task
#define STACK_SIZE 2048

#include "IPAddress.h"

/* Local DNS */
static const char *HOST = "vaio";

/* IP Address details */
static const IPAddress LOCAL_IP(192, 168, 1, 1);
static const IPAddress GATEWAY(192, 168, 1, 1);
static const IPAddress SUBNET(255, 255, 255, 0);

// Used NVS Namespace for DS4 MAC Address
static const char *MAC_ADDR_STORAGE_NAMESPACE = "DS4-NVS";

#endif
