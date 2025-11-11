/*
 * pid.c
 *
 *  Created on: Apr 8, 2025
 *      Author: Daniel
 */

#include "pid.h"
#include "driverlib.h"
#include "board.h"

// External variables
extern float VMain;
extern float VBatt;
extern float VCurr;
extern float VRef;
extern float IBatt;

PIDControl_t IBattFast = {.value = 0, .ref = 0, .error = 0, .error_old = 0, .Kp = 0.15, .Ki = 0.01, .output = 0.7, .min = 0.0, .max = 1.0};
PIDControl_t IBattSlow = {.value = 0, .ref = 0, .error = 0, .error_old = 0, .Kp = 0.15, .Ki = 0.01, .output = 0, .min = 0.0, .max = 1.0};
PIDControl_t IBattReg = {.value = 0, .ref = 0, .error = 0, .error_old = 0, .Kp = 0.15, .Ki = 0.01, .output = 0, .min = 0.0, .max = 1.0};

PIDControl_t VBuckReg = {.value = 0, .ref = 4.0, .error = 0, .error_old = 0, .Kp = 3.5, .Ki = 0.15, .output = 0, .min = -1.0, .max = 1.0};

PIDControl_t VBoostFast = {.value = 0, .ref = 5.0, .error = 0, .error_old = 0, .Kp = 40, .Ki = 1, .output = 6.0, .min = -1.0, .max = 6.0};
PIDControl_t VBoostSlow = {.value = 0, .ref = 5.0, .error = 0, .error_old = 0, .Kp = 10, .Ki = 0.1, .output = 0, .min = -1.0, .max = 6.0};
PIDControl_t VBoostReg = {.value = 0, .ref = 5.0, .error = 0, .error_old = 0, .Kp = 10, .Ki = 0.1, .output = 0, .min = -1.0, .max = 6.0};

//PIDControl_t VBuckSimple = {.value = 0, .ref = 0, .error = 0, .error_old = 0, .Kp = 0.01, .Ki = 0.001, .output = 0, .min = 0.0, .max = 1.0};

#pragma CODE_SECTION(PID, ".TI.ramfunc")
float PID(float ref, float value, PIDControl_t* pid) {
    // Store values (for debug purposes)
    pid->ref = ref;
    pid->value = value;
    // Error compared to the reference
    pid->error = ref - value;
    // Integrate
    pid->output += pid->Kp * (pid->error - pid->error_old) + pid->Ki * pid->error;
    // Store old error (to calculate change/derivate)
    pid->error_old = pid->error;
    // Saturate output
    pid->output = __fsat(pid->output, pid->max, pid->min);
    // return saturated output
    return pid->output;
}

// Dummy voltage to voltage based pwm control
/*
#pragma CODE_SECTION(BuckSimple, ".TI.ramfunc")
void BuckSimple(float vRef) {
    float pwm = PID(vRef, VBatt, &VBuckSimple);
    EPWM_setCounterCompareValue(GATE_PWM_BASE, EPWM_COUNTER_COMPARE_A, (1.0-pwm) * TBPRD);
}
*/

// DCDC converter current control, used by Buck and Boost voltage control (and current limiting)
#pragma CODE_SECTION(ControlCurrent, ".TI.ramfunc")
void ControlCurrent(float iRef) {
    float pwm = PID(iRef, IBatt, &IBattReg);
    EPWM_setCounterCompareValue(GATE_PWM_BASE, EPWM_COUNTER_COMPARE_A, (1.0-pwm) * TBPRD);
}

// DCDC converter Buck (voltage decreasing) mode
#pragma CODE_SECTION(ControlBuck, ".TI.ramfunc")
void ControlBuck() {
    float current = PID(VBuckReg.ref, VBatt, &VBuckReg);
    ControlCurrent(current);
}

// DCDC converter Boost (voltage increasing) mode
#pragma CODE_SECTION(ControlBoost, ".TI.ramfunc")
void ControlBoost() {
    float current = PID(VBoostReg.ref, VMain, &VBoostReg);
    float dutyCycle = -IBattReg.output;
    //float dutyCycle = -1 * VBatt / VMain;
    ControlCurrent(current * dutyCycle);
}
