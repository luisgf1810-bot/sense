#include "Battery.hpp"
#include "esp_log.h"


Battery::Battery() {

}

void Battery::Init(){
    pinMode(0, INPUT); /*Set up Cable indication pin*/
    pinMode(4, INPUT); /*Set up Battery Voltage Read pin*/
    analogSetAttenuation(ADC_11db);

    uint16_t battery_voltage = (((uint16_t)analogReadMilliVolts(4)) * 2U)+65U;

    if (battery_voltage < USB_VOLTAGE) {
        //_charge_color = LED_COLOR_GREEN;
    } else if (battery_voltage < MIN_BATTERY_VOLTAGE) {
        _chrg_counter = 0;
        
    } else {
        //_charge_color = LED_COLOR_BLUE;
    }
    for (uint8_t vv = 0; vv < AVRG_FILTER_SIZE; vv++) {
        _voltage_avrg[vv] = battery_voltage;
    }
}