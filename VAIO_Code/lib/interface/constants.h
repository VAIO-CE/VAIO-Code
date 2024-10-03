#ifndef CONSTANT_H
#define CONSTANT_H

// This is signal timeout in milli seconds.
#define SIGNAL_TIMEOUT 500

// define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

// sets speed of DC  motors
#define MAX_SPEED 190
#define MAX_SPEED_OFFSET 20

// FreeRTOS Task
#define STACK_SIZE 2048

/* Local DNS */
static const char *HOST = "vaio";

/* ESP32 SSID & Password */
static const char *SSID = "VAIO";
static const char *PASSWORD = "skibiditoilet123";

/* IP Address details */
static const IPAddress LOCAL_IP(192, 168, 1, 1);
static const IPAddress GATEWAY(192, 168, 1, 1);
static const IPAddress SUBNET(255, 255, 255, 0);

#endif