//#############################################################################
//
// FILE:   main.c
//
//! This project is the firmware for the Bidirectional Power Supply.
//! The peripherials (ePWM, ADC, GPIO, TIMER, SCI)
//! are configured with sysconfig.
//! The main function controls the duty cycle of the pwm.
//
// Created on: 2024.11.17.
// Author: Daniel Sinko
//
//#############################################################################

//
// Included Files
//
#include <stdio.h>
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "c2000ware_libraries.h"

#include "MUCI_bsp.h"



//
// Globals
//

// Flags
bool timerFlag = 0;
bool adcFlag = 0;

// Counters
uint32_t BackgndCounter = 0;
uint32_t pwmTzIntCnt = 0;
uint32_t adcIntCnt= 0;
uint32_t timerIntCnt = 0;
uint32_t ledCnt = 0;
uint32_t CmpaVal = 600;
uint32_t CmpaMax = 156;
uint32_t CmpaCnt = 0;



#define BUFFER_SIZE 256
uint16_t index = 0;

// Adc values
uint16_t Adc5V;
uint16_t AdcBatt;
uint16_t AdcCurrent;
uint16_t AdcRef;
uint16_t AdcRefBuff[BUFFER_SIZE];

float V5v;
float VBatt;
float VCurr;
float VRef;
float I;

// Control variables
uint32_t IOTestMode = 1;
uint32_t StartPWM = 0;
uint32_t StartPWMJump = 0;
uint32_t StartState = 0;
uint32_t StopPWM = 0;
uint32_t enableGate = 0;
uint32_t disableGate = 0;
uint32_t ackTripZone = 0;

uint16_t ControlEnabled = 0;
float Iref = 0;
float Cur_reg_error = 0;
float Cur_reg_error_old = 0;
float Cur_reg_P = 0;
float Cur_reg_I = 0;
float Cur_reg_output = 0;
float Cur_reg_min = 0.05;
float Cur_reg_max = 0.95;


//
// Interrupt Function Prototypes
//
__interrupt void INT_TIMER_1MS_ISR(void);
__interrupt void INT_GATE_PWM_TZ_ISR(void);
__interrupt void INT_ADC0_1_ISR(void);

//
// Function Prototypes
//
void EnableAll();
void Control();
void Application();
void Blink(uint32_t pin, uint32_t cntMax, uint32_t* cnt);
void TimerDelay(uint32_t cntMax, uint32_t* cnt);


//
// Main
//
void main(void)
{
    // Initialize device clock and peripherals
    Device_init();

    // Disable pin locks and enable internal pull-ups.
    Device_initGPIO();

    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    Interrupt_initModule();

    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    Interrupt_initVectorTable();

    // PinMux and Peripheral Initialization
    Board_init();

    // Disable ePWM1 by default
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);

    // Initialize MUCI variable monitoring
    MUCI_bsp_Init();

    // Enable Global Interrupt (INTM) and real time interrupt (DBGM)
    EINT;
    ERTM;

    while(1) {
        MUCI_bsp_Background();
        BackgndCounter++;

        if(IOTestMode) {
            Control();
        } else {
            EnableAll();
        }

        /*
        if(adcFlag) {
            adcFlag = 0;

        }
        */

        if(timerFlag) {
            timerFlag = 0;

            Application();
            Blink(LED2, 1000, &ledCnt);
        }
    }
}

//
// Interrupt Functions
//

// 1ms timer interrupt for timing
__interrupt void INT_TIMER_1MS_ISR(void)
{
    timerFlag = 1;
    timerIntCnt++;

    // The CPU acknowledges the interrupt.
}

// ePWM Trip-Zone triggered interrupt
__interrupt void INT_GATE_PWM_TZ_ISR(void)
{
    pwmTzIntCnt++;

    // Disable gate outputs and pwm
    GPIO_writePin(GATE_EN, 0);
    //SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);

    // Wait to acknowledge the Trip-Zone
    while(!ackTripZone) {
        ackTripZone = 0;
        MUCI_bsp_Background();
        EPWM_clearTripZoneFlag(GATE_PWM_BASE, (EPWM_TZ_INTERRUPT | EPWM_TZ_FLAG_OST));
    }

    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);

    // Acknowledge this interrupt to receive more interrupts from group
    Interrupt_clearACKGroup(INT_GATE_PWM_TZ_INTERRUPT_ACK_GROUP);
}

// ADC end of conversion interrupt
// ADC conversion is generated at every ePWM ZRO/PRD events
#pragma CODE_SECTION(INT_ADC0_1_ISR, ".TI.ramfunc")
__interrupt void INT_ADC0_1_ISR(void)
{
    adcFlag = 1;
    adcIntCnt++;

    // Add the latest result to the buffer
    AdcCurrent = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);
    Adc5V = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER1);
    AdcRef = ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER0);
    //AdcRefBuff[(index++)%BUFFER_SIZE] = AdcRef;
    AdcBatt = ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER1);

    VCurr = (float)AdcCurrent * 0.000805664; // AdcCurrent/4096 * 3.3
    I = -(VCurr-VRef)/0.2f;

    Cur_reg_error = Iref - I;

    if(ControlEnabled) {
        Cur_reg_output += Cur_reg_error * Cur_reg_I + (Cur_reg_error - Cur_reg_error_old) * Cur_reg_P;
        Cur_reg_error_old = Cur_reg_error;
        if(Cur_reg_output > Cur_reg_max)
            Cur_reg_output = Cur_reg_max;
        else if(Cur_reg_output < Cur_reg_min)
            Cur_reg_output = Cur_reg_min;
        EPWM_setCounterCompareValue(GATE_PWM_BASE, EPWM_COUNTER_COMPARE_A, (1.0-Cur_reg_output) * 600);
    }

    MUCI_Recorder_Trigger();

    // Clear the interrupt flag
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);

    // Check if overflow has occurred
    if(true == ADC_getInterruptOverflowStatus(ADCA_BASE, ADC_INT_NUMBER1))
    {
        ADC_clearInterruptOverflowStatus(ADCA_BASE, ADC_INT_NUMBER1);
        ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
        GPIO_togglePin(LED1);
    }

    // Acknowledge the interrupt
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

//
// Functions
//

// Enable ALL peripherals
void EnableAll() {
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);
    GPIO_writePin(GATE_EN, 1);
}

// Function to toggle peripherals
void Control() {
    //GPIO_togglePin(LED1);

    if(StartPWM) {
        StartPWM = 0;
        //StartState = 1;
        SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);
    }

    if(StartPWMJump) {
        StartPWMJump = 0;
        SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);
        EPWM_setCounterCompareValue(GATE_PWM_BASE, EPWM_COUNTER_COMPARE_A, 156);
        //EPWM_forceTripZoneEvent(base, tzForceEvent)
    }

    if(StopPWM) {
        StopPWM = 0;
        //StartState = 0;
        //CmpaVal = 600;
        SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);
    }

    if(enableGate) {
        enableGate = 0;
        GPIO_writePin(GATE_EN, 1);
    }

    if(disableGate) {
        disableGate = 0;
        GPIO_writePin(GATE_EN, 0);
    }
}

// Main calculations
void Application() {
    V5v = (float)Adc5V * 0.001371343; // Adc5V/4096 * 3.3 * (4.7+3.3)/4.7
    VBatt = (float)AdcBatt * 0.001371343; // AdcBatt/4096 * 3.3 * (4.7+3.3)/4.7
//    uint32_t buffer = 0;
//    uint32_t i;
//    for(i = 0; i < BUFFER_SIZE; i++) {
//        buffer += AdcRefBuff[i];
//    }
//    buffer = buffer;
    VRef = (float)AdcRef * 0.000805664; // AdcRef/4096 * 3.3

    /*
    if(StartState) {
        if(CmpaVal > CmpaMax) {
            TimerDelay(25, &CmpaCnt);
        }

    }
    */
}

// A function to toggle the LED pins
void Blink(uint32_t pin, uint32_t cntMax, uint32_t* cnt)
{
    (*cnt)++;
    if(cntMax <= *cnt) {
        *cnt = 0;

        GPIO_togglePin(pin);
    }
}

void TimerDelay(uint32_t cntMax, uint32_t* cnt) {
    (*cnt)++;
    if(cntMax <= *cnt) {
        *cnt = 0;

        CmpaVal--;
        EPWM_setCounterCompareValue(GATE_PWM_BASE, EPWM_COUNTER_COMPARE_A, CmpaVal);
    }
}



//
// End of File
//
