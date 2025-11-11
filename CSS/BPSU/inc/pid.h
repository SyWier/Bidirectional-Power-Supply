/*
 * pid.h
 *
 *  Created on: Apr 8, 2025
 *      Author: Daniel
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#define TBPRD 600

// PDI structure
// Kp = Ap
// Ki = Ap * (Ts / Ti)
typedef struct {
    float value;
    float ref;
    float error;
    float error_old;
    float Kp;
    float Ki;
    float output;
    float min;
    float max;
} PIDControl_t;

/* IBattReg
 * .ref: Desired current flowing in
 * .output: Duty cycle to achieve the reference*/
extern PIDControl_t IBattReg;
/* VBuckReg: VMain to VBatt
 * .ref: Desired voltage of the buck converter
 * .output: Current to achieve the reference */
extern PIDControl_t VBuckReg;
/* VBoostReg: VBatt to VMain
 * .ref: Desired voltage of the boost converter
 * .output: Current to achieve the reference */
extern PIDControl_t VBoostReg;

float PID(float ref, float value, PIDControl_t* pid);
void ControlCurrent(float iRef);
void ControlBuck();
void ControlBoost();

#endif /* INC_PID_H_ */
