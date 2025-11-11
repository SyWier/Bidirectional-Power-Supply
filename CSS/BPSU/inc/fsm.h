/*
 * fsm.h
 *
 *  Created on: Apr 8, 2025
 *      Author: Daniel
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

typedef enum {
    BOOT,
    STANDBY,
    EXTERNAL,
    NORMAL,
    DISCHARGING,
    FAULT
} PowerState;
extern PowerState powerState;

typedef enum {
    PRECHARGING,
    CHARGING,
    CHARGED
} BatteryState;
extern BatteryState batteryState;

typedef enum {
    FSMERR_NONE = 0,
    FSMERR_SHORT_CIRCUIT = 1<<0, // Trip Zone: current too high
    FSMERR_BATT_UV = 1<<1, // Too low voltage to charge
    FSMERR_BATT_OV = 1<<2, // Too high voltage
    FSMERR_USB_UV = 1<<3,
    FSMERR_USB_OV = 1<<4,
    FSMERR_UNKNOWN_PWR_STATE = 1<<5,
    FSMERR_UNKNOWN_BATT_STATE = 1<<6,
    FSMERR_ERR = 1<<7
} FSM_ERR_t;
extern FSM_ERR_t fsm_err;

bool FaultCheck();
void UpdatePowerState();
void UpdateBatteryState();
void HandlePowerStates();
void HandleBatteryStates();

#endif /* INC_FSM_H_ */
