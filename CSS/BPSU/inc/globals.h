/*
 * globals.h
 *
 *  Created on: Apr 8, 2025
 *      Author: Daniel
 */

#ifndef INC_GLOBALS_H_
#define INC_GLOBALS_H_

typedef struct {
    float maxVoltage;  // Maximum voltage (5.25V)
    float nominalVoltage; // Nominal voltage (5.0V)
    float lowVoltage; // Low voltage (4.9V)
    float minVoltage;  // Minimum voltage (4.75V)
    float noVoltage; // No USB power input (4.0V)
} USBVoltageRange;
extern USBVoltageRange usbPower;

typedef struct { // C = 3400mAh
    float capacity; // 3350mAh * 95% @ 2C discharge current

    float voltage; // Recommended maximum charge voltage for UPS usage (4V)
    float nominalVoltage; // Nominal battery voltage (3.7V)
    float prechargeVoltage; // Precharge under (3V)
    float overVoltage; // Maximum voltage (4.2V)
    float underVoltage; // Minimum voltage to charge (2V) ~ 1V
    float noVoltage; // Battery is not present (0.5V)

    float prechargeCurrent; // ~0.1C precharge current
    float chargeCurrent; // ~0.5C charge current
    float dischargeCurrent; // ~2C discharge current
    float cutoffCurrent; // ~0.025C when battery is charged
} Battery_t;
extern Battery_t battery;

#endif /* INC_GLOBALS_H_ */
