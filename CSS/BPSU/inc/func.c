/*
 * func.c
 *
 *  Created on: Apr 27, 2025
 *      Author: Daniel
 */

#include "board.h"
#include "func.h"
#include "globals.h"
#include "fsm.h"

extern uint32_t ledCnt;
extern bool tzFlag;
extern State_t pwmState;

// A function to toggle the LED pins
void Blink(uint32_t pin, uint32_t cntMax, uint32_t* cnt) {
    (*cnt) += 1;
    if(cntMax <= *cnt) {
        *cnt = 0;
        GPIO_togglePin(pin);
    }
}

// Update LEDs based on the state machine
void LedState() {
    switch(powerState) {
    case BOOT:
        GPIO_writePin(LED1, 0);
        GPIO_writePin(LED2, 0);
        return;
    case STANDBY:
        GPIO_writePin(LED1, 1);
        GPIO_writePin(LED2, 0);
        return;
    case EXTERNAL:
        GPIO_writePin(LED1, 0);
        GPIO_writePin(LED2, 1);
        return;
    case NORMAL:
        if(batteryState == PRECHARGING) {
            Blink(LED1, 1000, &ledCnt);
            GPIO_writePin(LED2, 1);
            return;
        }

        if(batteryState == CHARGING) {
            Blink(LED1, 500, &ledCnt);
            GPIO_writePin(LED2, 1);
            return;
        }

        if(batteryState == CHARGED) {
            GPIO_writePin(LED1, 1);
            GPIO_writePin(LED2, 1);
            return;
        }
        //...
        return;
    case DISCHARGING:
        Blink(LED1, 500, &ledCnt);
        GPIO_writePin(LED2, 0);
        return;
    case FAULT:
        Blink(LED1, 1000, &ledCnt);
        GPIO_writePin(LED2, !GPIO_readPin(LED1));
        return;
    default:
        // Unknown state ...
        Blink(LED1, 500, &ledCnt);
        GPIO_writePin(LED2, !GPIO_readPin(LED1));
        return;
    }
}

// Enable/Disable ePWM gate control
#pragma CODE_SECTION(GateControl, ".TI.ramfunc")
void GateControl(bool enable) {
    if(enable)  {
        pwmState.state = 1;
        tzFlag = 0;
        EPWM_clearTripZoneFlag(GATE_PWM_BASE, EPWM_TZ_FLAG_OST);
    } else {
        pwmState.state = 0;
        tzFlag = 1;
        EPWM_forceTripZoneEvent(GATE_PWM_BASE, EPWM_TZ_FORCE_EVENT_OST);
    }
}
