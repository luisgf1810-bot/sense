#include "Arduino.h"
#include "bno085.hpp"
#include "LedStrip.hpp"
#include "Battery.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_log.h"

#include "esp_netif.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_mac.h"


#define WIFI_SSID      "Rocky211"
#define WIFI_PASS      "raspberry"



#define LED_PIN 19U
#define LED_ON_PIN 20U
#define SENS_ON_PIN 18U
#define MOTION_WAKEUP_PIN 7U
#define LIGHT_WAKEUP_PIN 5U


void Motion_Init();

// Logs
static const char *MOTION_TAG = "motion_task";
static const char *MAIN = "main";


float _motion_data[23] = { 0.0 };

uint8_t _i2c_write_array[10] = { 0 };
uint8_t _i2c_read_array[10] = { 0 };
uint8_t _i2c_write_size = 0;

float x = 0.0;  // X-axis acceleration
float y = 0.0;  // Y-axis acceleration
float z = 0.0;  // Z-axis acceleration

static int64_t start_time, end_time  = 0;  // Start time for motion reading
static int s_retry_num = 0;

static EventGroupHandle_t wifi_event_group;
const int WIFI_CONNECTED_BIT = BIT0;

// Motion, battery and led sensors
static bno085 Motion;
static Battery battery;
static LedStrip led_strip;

