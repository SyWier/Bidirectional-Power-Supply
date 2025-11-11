/*
 * globals.c
 *
 *  Created on: Apr 8, 2025
 *      Author: Daniel
 */

#include "globals.h"

USBVoltageRange usbPower = {
    .maxVoltage = 5.25,
    .nominalVoltage = 5.0,
    .lowVoltage = 4.85,
    .minVoltage = 4.75,
    .noVoltage = 4.0
};

Battery_t battery = { // C = 3400mAh
    .capacity = 3180, // 3350mAh * 95% @ 2C discharge current

    .voltage = 4.0, // Recommended maximum charge voltage for UPS usage (4V)
    .nominalVoltage = 3.7, // Nominal battery voltage (3.7V)
    .prechargeVoltage = 3.0, // Precharge under (3V)
    .overVoltage = 4.2, // Maximum voltage (4.2V)
    .underVoltage = 2.0, // Minimum voltage to charge (2V) ~ 1V
    .noVoltage = 0.5, // Battery is not present (0.5V)

    .prechargeCurrent = 0.3, // ~0.1C precharge current
    .chargeCurrent = 1.0, // ~0.5C charge current
    .dischargeCurrent = 6.0, // ~2C discharge current
    .cutoffCurrent = 0.080, // ~0.025C when battery is charged
};
