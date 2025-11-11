/*
 * fsm.c
 *
 *  Created on: Apr 8, 2025
 *      Author: Daniel
 */

#include "board.h"

#include "fsm.h"
#include "globals.h"
#include "pid.h"
#include "func.h"

// External variables
extern bool tzFlag;
extern bool timer5SecFlag;
extern uint32_t timer5Cnt;

extern float VMain;
extern float VBatt;
extern float VCurr;
extern float VRef;
extern float IBatt;
extern float VMainAvg;

extern PIDControl_t IBattFast;
extern PIDControl_t IBattSlow;
extern PIDControl_t VBoostFast;
extern PIDControl_t VBoostSlow;

PowerState powerState = BOOT;
BatteryState batteryState = CHARGED;
FSM_ERR_t fsm_err = FSMERR_NONE;

// Check for faults before making decisions
bool FaultCheck() {
    // Short circuit
    // TripZone is not flagged by software but TZ is flagged
    if(tzFlag == 0 && (EPWM_getTripZoneFlagStatus(GATE_PWM_BASE) & EPWM_TZ_FLAG_OST) == 1) {
        powerState = FAULT;
        fsm_err |= FSMERR_SHORT_CIRCUIT;
        return 1;
    }

    // Battery voltage too low to charge
    if(battery.noVoltage < VBatt && VBatt < battery.underVoltage) {
        powerState = FAULT;
        fsm_err |= FSMERR_BATT_UV;
        return 1;
    }

    // Battery voltage too high
    if(battery.overVoltage < VBatt) {
        powerState = FAULT;
        fsm_err |= FSMERR_BATT_OV;
        return 1;
    }

    // USB voltage is bellow the minimum voltage (while USB voltage is present)
    if(usbPower.noVoltage < VMain && VMain < usbPower.minVoltage) {
        powerState = FAULT;
        fsm_err |= FSMERR_USB_UV;
        return 1;
    }

    // USB voltage is above maximum voltage
    if(usbPower.maxVoltage < VMain) {
        powerState = FAULT;
        fsm_err |= FSMERR_USB_OV;
        return 1;
    }

    return 0;
}

// Update the power state machine
void UpdatePowerState() {
    FaultCheck();

    switch(powerState) {
    case BOOT:
        // Reset register values
        IBattReg = IBattSlow;
        VBoostReg = VBoostSlow;

        // Main voltage with battery present
        if(usbPower.lowVoltage < VMain && battery.underVoltage < VBatt) {
            powerState = NORMAL;
            return;
        }

        // No main voltage with battery present
        if(VMain < usbPower.lowVoltage && battery.underVoltage < VBatt) {
            powerState = STANDBY;
            return;
        }

       // Main voltage with no battery
        if(usbPower.lowVoltage < VMain && VBatt < battery.noVoltage) {
            powerState = EXTERNAL;
            return;
        }

        powerState = FAULT; // Insufficient power state
        fsm_err |= FSMERR_UNKNOWN_PWR_STATE;
        return;

    case STANDBY:
        // USB power is back
        if(usbPower.lowVoltage < VMain) {
            powerState = NORMAL;
            return;
        }

        powerState = STANDBY;
        return;

    case EXTERNAL:
        // Battery is present
        if(battery.underVoltage < VBatt) {
            powerState = NORMAL;
            return;
        }

        powerState = EXTERNAL;
        return;

    case NORMAL:
        /*
        // USB power is down
        //if(VMain < VMainAvg - 0.05) {
        if(VMain < usbPower.lowVoltage) {
            powerState = DISCHARGING;
            return;
        }
        */

        // Battery is not present
        if(VBatt < battery.noVoltage) {
            powerState = EXTERNAL;
            return;
        }

        UpdateBatteryState();

        powerState = NORMAL;
        return;

    case DISCHARGING:
        if(timer5Cnt >= 50) { // Use slow parameter set after 5ms
            IBattReg.Kp = IBattSlow.Kp;
            IBattReg.Ki = IBattSlow.Ki;
            VBoostReg.Kp = VBoostSlow.Kp;
            VBoostReg.Ki = VBoostSlow.Ki;
        }

        // Battery shouldn't be discharged too low
        if(VBatt < battery.prechargeVoltage) {
            powerState = STANDBY;
            return;
        }

        // Test if Main voltage is back
        if(timer5SecFlag) {
            timer5SecFlag = 0;
            powerState = NORMAL;
            return;
        }

        // USB power is back
        /*
        if(usbPower.lowVoltage < VMain) {
            powerState = NORMAL;
            return;
        }
        */

        powerState = DISCHARGING;
        return;

    case FAULT:
        // Restore state if no error is present
        if(timer5SecFlag) {
            timer5SecFlag = 0;

            if(!FaultCheck()) {
                powerState = BOOT;
                return;
            }
        }

        powerState = FAULT;
        return;

    default:
        // Unknown power state
        powerState = FAULT;
        fsm_err |= FSMERR_UNKNOWN_PWR_STATE;
        return;

    }
}
// Update the battery state machine
void UpdateBatteryState() {
    switch(batteryState) {
    case PRECHARGING:
        // Can be charged normally if the voltage is high enough
        if(battery.prechargeVoltage < VBatt) {
            batteryState = CHARGING;
            return;
        }

        batteryState = PRECHARGING;
        return;

    case CHARGING:
        // Battery is charged if little to no current flows
        if(timer5SecFlag && IBatt < battery.cutoffCurrent) {
            batteryState = CHARGED;
            return;
        }

        batteryState = CHARGING;
        return;

    case CHARGED:
        // If the voltage is below the normal level, start charging
        if(timer5SecFlag && VBatt < battery.nominalVoltage) {
            batteryState = CHARGING;
            return;
        }

        batteryState = CHARGED;
        return;

    default:
        // Unknown battery state
        powerState = FAULT;
        fsm_err |= FSMERR_UNKNOWN_BATT_STATE;
        return;
    }
}
// Handle the appropriate states
#pragma CODE_SECTION(HandlePowerStates, ".TI.ramfunc")
void HandlePowerStates() {
    // USB power is down
    //if( ( powerState == NORMAL ) && ( VMain < (VMainAvg - 0.05f) ) ) {
    //if(powerState == NORMAL && ( VMain < usbPower.lowVoltage || VMain < (VMainAvg-0.05f) ) ) {
    if(powerState == NORMAL && VMain < usbPower.lowVoltage) {
        powerState = DISCHARGING;
        IBattReg = IBattFast;
        VBoostReg = VBoostFast;
    }

    switch(powerState) {
    case BOOT:
        GateControl(0);
        return;
    case STANDBY:
        // Turn off battery to preserve power
        GateControl(0);
        return;
    case EXTERNAL:
        GateControl(0);
        return;
    case NORMAL:
        HandleBatteryStates();
        return;
    case DISCHARGING:
        ControlBoost();
        GateControl(1);
        return;
    case FAULT:
        // Disable ePWM output
        GateControl(0);
        return;
    default: // How Did We Get Here?
        powerState = FAULT;
        fsm_err |= FSMERR_UNKNOWN_PWR_STATE;
        GateControl(0);
        return;
    }
}

#pragma CODE_SECTION(HandleBatteryStates, ".TI.ramfunc")
void inline HandleBatteryStates() {
    switch(batteryState) {
    case PRECHARGING: // Charge reeeealy slowlyyy
        VBuckReg.max = 0.25;
        ControlBuck();
        GateControl(1);
        return;
    case CHARGING: // Charging 8)
        VBuckReg.max = 1;
        ControlBuck();
        GateControl(1);
        return;
    case CHARGED:
        GateControl(0);
        return; // All good, nothing to do here
    default:
        powerState = FAULT;
        fsm_err |= FSMERR_UNKNOWN_BATT_STATE;
        return;
    }
}
