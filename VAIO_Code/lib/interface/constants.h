#ifndef CONSTANT_H
#define CONSTANT_H

// define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

// Setting PWM properties
#define PWM_Channel_Left 1
#define PWM_Channel_Right 2
#define PWM_Frequency 30000
#define PWM_Resolution  8

//Setting Motor Speed properties
#define MIN_SPEED_MOTOR 155
#define MAX_SPEED_MOTOR 255

// FreeRTOS Task
#define STACK_SIZE 2048

#include "IPAddress.h"

/* Local DNS */
static constexpr char *HOST = "vaio";

/* MAC Address Glove */
 static constexpr uint8_t GloveAddress[] {0x14, 0x2B, 0x2F, 0xD7, 0x64, 0x3C}; // Original Glove Address
// static const uint8_t GloveAddress[] = {0xC4, 0xDE, 0xE2, 0x1F, 0x88, 0x84}; // Seno Glove Address

/* IP Address details */
static const IPAddress LOCAL_IP(192, 168, 1, 1);
static const IPAddress GATEWAY(192, 168, 1, 1);
static const IPAddress SUBNET(255, 255, 255, 0);

// Used NVS Namespace for DS4 MAC Address
static constexpr char *MAC_ADDR_STORAGE_NAMESPACE {"DS4-NVS"};

#endif
