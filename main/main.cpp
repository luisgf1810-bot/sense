//file: main.cpp
#include "Arduino.h"
#include "bno085.hpp"
#include "esp_log.h"


#define LED_PIN 19U
#define LED_ON_PIN 20U
#define SENS_ON_PIN 18U
#define MOTION_WAKEUP_PIN 7U
#define LIGHT_WAKEUP_PIN 5U

// Logs
static const char *TAG = "main";

// Motion sensor
bno085 Motion;
float _motion_data[23] = { 0.0 };

uint8_t _i2c_write_array[10] = { 0 };
uint8_t _i2c_read_array[10] = { 0 };
uint8_t _i2c_write_size = 0;

float x = 0.0;  // X-axis acceleration
float y = 0.0;  // Y-axis acceleration
float z = 0.0;  // Z-axis acceleration



void Motion_Read() {
  bool error_flag = 1;
  uint8_t motion_id = 0xFF;

  while (Motion.getSensorEvent() == true) {
      motion_id = Motion.getSensorEventID();
      if (motion_id == SENSOR_REPORTID_ROTATION_VECTOR) {
          _motion_data[0] = Motion.getRot_R();
          _motion_data[1] = Motion.getRot_I();
          _motion_data[2] = Motion.getRot_J();
          _motion_data[3] = Motion.getRot_K();
          error_flag = 0;
      }
      if (motion_id == SENSOR_REPORTID_ACCELEROMETER) {
          _motion_data[8] = Motion.getAccelX();
          _motion_data[9] = Motion.getAccelY();
          _motion_data[10] = Motion.getAccelZ();
          error_flag = 0;
      }
      if (motion_id == SENSOR_REPORTID_GYROSCOPE_CALIBRATED) {
          _motion_data[11] = Motion.getGyroX();
          _motion_data[12] = Motion.getGyroY();
          _motion_data[13] = Motion.getGyroZ();
          error_flag = 0;
      }
      if (motion_id == SENSOR_REPORTID_MAGNETIC_FIELD) {
          _motion_data[14] = Motion.getMagX();
          _motion_data[15] = Motion.getMagY();
          _motion_data[16] = Motion.getMagZ();
          error_flag = 0;
      }
      if (motion_id == SENSOR_REPORTID_GRAVITY) {
          _motion_data[17] = Motion.getGravityX();
          _motion_data[18] = Motion.getGravityY();
          _motion_data[19] = Motion.getGravityZ();
          error_flag = 0;
      }
      if (motion_id == SENSOR_REPORTID_LINEAR_ACCELERATION) {
          _motion_data[20] = Motion.getLinAccelX();
          _motion_data[21] = Motion.getLinAccelY();
          _motion_data[22] = Motion.getLinAccelZ();
          error_flag = 0;
      }

      if (error_flag) {
          ESP_LOGI(TAG, ">> Error: Motion Sensor not found");
      }
    }
}

void Motion_Init() {

  Wire.begin(8, 9, 400000);  // SDA on GPIO8, SCL on GPIO9, 400kHz speed

  if (Motion.begin() == false) {
    ESP_LOGI(TAG, ">> Error: Motion Sensor not found - Check Hardware");
  }

  if (Motion.isConnected() == false) {
    ESP_LOGI(TAG, ">> Error: Motion Sensor not found");
  }
 

  if (Motion.enableLinearAccelerometer() == true) {
      ESP_LOGI(TAG, "Linear Accelerometer Activated | ");
    } else {
      ESP_LOGI(TAG, "Linear Accelerometer Motion: Failed | ");
  }

  _i2c_write_size = 0;
}

void Motion_AccelerometerRead(float &x, float &y, float &z) {
    x = _motion_data[8];
    y = _motion_data[9];
    z = _motion_data[10];
}

void Motion_LinearAccRead(float &x, float &y, float &z) {
    x = _motion_data[20];
    y = _motion_data[21];
    z = _motion_data[22];
}

void PrintSensors() {
    float xx = 0;
    float yy = 0;
    float zz = 0;

    ESP_LOGI(TAG, ">> Reading Sensors: ");

    Motion_AccelerometerRead(xx, yy, zz);
    ESP_LOGI(TAG, " Acc XYZ: %.2f, %.2f, %.2f m/s^2 |", xx, yy, zz);

    Motion_LinearAccRead(xx, yy, zz);
    ESP_LOGI(TAG, " Linear Acc XYZ: %.2f, %.2f, %.2f m/s^2 |", xx, yy, zz);
}



extern "C" void app_main()
{
  initArduino();

  // Setup sensors enable pin
  ESP_LOGI(TAG, "Initialing sensors ...");
  pinMode(LED_ON_PIN, OUTPUT);    /*Set LED transistor pin as output*/
  digitalWrite(LED_ON_PIN, HIGH); /*Init Set up to output high*/
  pinMode(SENS_ON_PIN, OUTPUT); /*Set LDO enable pin as output*/
  gpio_hold_dis((gpio_num_t)SENS_ON_PIN);
  digitalWrite(SENS_ON_PIN, HIGH); /*Init Set up to output high*/
  pinMode(LIGHT_WAKEUP_PIN, INPUT);   // Assumes active-low button
  pinMode(MOTION_WAKEUP_PIN, INPUT);  // Assumes active-low button

  Serial.begin(115200);
  delay(1500);

  // init motion reports
  Motion_Init();
  Motion_Read();
  Motion_Read();
  PrintSensors();
  
  while(true){
    
    /*x = Motion.getLinAccelX();
    y = Motion.getLinAccelY();
    z = Motion.getLinAccelZ();*/
    
    delay(1000);
  }

  // WARNING: if program reaches end of function app_main() the MCU will restart.
}