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
#include <stdbool.h>
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "func.h"
#include "c2000ware_libraries.h"

typedef unsigned int Uint16;
#include "f28003x_piectrl.h"

#include "MUCI_bsp.h"

#include "pid.h"
#include "globals.h"
#include "fsm.h"

//
// Globals
//

// Flags
bool timer100uSecFlag = 0;
bool timer1mSecFlag = 0;
bool timer5SecFlag = 0;
bool adcFlag = 0;
bool tzFlag = 0;

// Counters
uint32_t BackgndCounter = 0;
uint32_t timer1Cnt = 0;
uint32_t timer5Cnt = 0;
uint32_t tzIntCnt = 0;
uint32_t adcIntCnt= 0;
uint32_t ledCnt = 0;
uint32_t ellapsedTime = 0;
uint32_t lastTime = 0;

// Adc values
typedef struct {
    uint16_t Main;
    uint16_t Batt;
    uint16_t Current;
    uint16_t Ref;
} adcStruct_t;
adcStruct_t adc;

float VMain;
float VMainAvg;
float VMainBuff[16];
uint8_t VMainBuffCnt;
float VBatt;
float VCurr;
float VRef;
float VLimit = 4.85;
float IBatt;
float PWMVal = 0.7;

// Control variables
uint32_t StateTestMode = 0;
#define HANDLE_TEST_MODE 0
uint32_t HandleTestMode = 0; // Currently not in use
uint32_t ackTripZone = 0;
uint16_t ControlEnabled = 0;
uint8_t MuciEnable = 0;

State_t gateState = {.state = 0, .enable = 0, .disable = 0};
State_t pwmState = {.state = 0, .enable = 0, .disable = 0};

typedef struct {
    bool buck;
    bool boost;
    bool current;
    bool pwm;
    bool buckPwm;
} enable_t;
enable_t enReg;

typedef struct {
    uint32_t t[10];
} timer_t;
volatile timer_t timer;
#define TIMER 1
#define TIMER_PRD 10000
#define TIMER_READ_CYCLES 7
uint8_t timerCnt = 0;

void inline SampleTime() {
#if TIMER
    timer.t[timerCnt] = TIMER_PRD - CPUTimer_getTimerCount(TIMER_0_BASE) - timerCnt*TIMER_READ_CYCLES;
    timerCnt = timerCnt + 1;
#endif
}

void inline SampleTimeReset() {
#if TIMER
    CPUTimer_reloadTimerCounter(TIMER_0_BASE);
    // Wait two cycle to take effect
    asm("       NOP");
    asm("       NOP");
    timerCnt = 0;
#endif
}



//
// Interrupt Function Prototypes
//
__interrupt void INT_TIMER_1MS_ISR(void);
__interrupt void INT_ADC0_1_ISR(void);

//
// Function Prototypes
//
void Application();
void Control();

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
    //SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);

    // Initialize MUCI variable monitoring
    MUCI_bsp_Init();

    // Disable PWM output, enable gate,
    GateControl(0);
    GPIO_writePin(GATE_EN, 1);
    gateState.state = 1;

    // Enable Global Interrupt (INTM) and real time interrupt (DBGM)
    EINT;
    ERTM;

    while(1) {
        BackgndCounter++;
        MUCI_bsp_Background();

        Control();

        if(timer100uSecFlag) {
            timer100uSecFlag = 0;

            if(!MuciEnable) {
                MUCI_Recorder_Trigger();
            }
        }

        if(timer1mSecFlag) {
            timer1mSecFlag = 0;

            // Update states manually if { StateTestMode = 1 }
            if(!StateTestMode) {
                UpdatePowerState();
            }
            Application();
        }
    }
}

//
// Interrupt Functions
//

// 1ms timer interrupt for timing
// Extra 8 cycles to enable nesting vs ISR routine + return ...
#pragma CODE_SECTION(INT_TIMER_1MS_ISR, ".TI.ramfunc")
__interrupt void INT_TIMER_1_ISR(void) {
    // === Enables IT priority for ADC_IT ===
    // Save PIEIER register for later
    uint16_t TempPIEIER;
    TempPIEIER = PieCtrlRegs.PIEIER1.all;

    // Enable Group 1 interrupts
    IER |= 0x0001;
    IER &= 0x0001;

    // Set group priority to allow INT1.1 to interrupt current ISR
    PieCtrlRegs.PIEIER1.all &= 0x0001;
    PieCtrlRegs.PIEACK.all = 0xFFFF; // Enable PIE interrupts

    // Clear INTM to enable interrupts
    asm("       NOP"); // Wait one cycle
    EINT;
    // === === === === ===

    // ISR code...
    timer100uSecFlag = 1;

    static PowerState lastState = BOOT;

    if(lastState != powerState) {
        lastState = powerState;
        timer5SecFlag = 0;
        timer5Cnt = 0;
    }

    timer5Cnt++;
    if(timer5Cnt >= 50000) {
        timer5SecFlag = 1;
        timer5Cnt = 0;
    }

    timer1Cnt++;
    if(timer1Cnt >= 10) {
        timer1mSecFlag = 1;
        timer1Cnt = 0;
    }

    // Restore state
    DINT;
    PieCtrlRegs.PIEIER1.all = TempPIEIER;

    // The CPU acknowledges the interrupt.
}

// ADC end of conversion interrupt
// ADC conversion is generated at every ePWM ZRO/PRD events
#pragma CODE_SECTION(INT_ADC0_1_ISR, ".TI.ramfunc")
__interrupt void INT_ADC0_1_ISR(void) {
    SampleTimeReset();
    SampleTime();

    adcFlag = 1;
    adcIntCnt++;

    // Read ADC
    adc.Current = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);
    adc.Main = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER1);
    adc.Ref = ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER0);
    adc.Batt = ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER1);

    SampleTime();

    // Calculate voltages
    // Other 2 value calculated at the main Application every 1ms
    VMain = (float)adc.Main * 0.001371343; // Adc.Main/4096 * 3.3 * (4.7+3.3)/4.7
    VBatt = (float)adc.Batt * 0.001371343; // Adc.Batt/4096 * 3.3 * (4.7+3.3)/4.7
    VCurr = (float)adc.Current * 0.000805664; // Adc.Current/4096*3.3
    IBatt = -(VCurr-VRef) * 5; // -(VCurr-VRef) / 0.2f;

    SampleTime();

    // Manual/Automatic conversion control
#if HANDLE_TEST_MODE
    if(enReg.buck)          ControlBuck(VBuckReg.ref);
    else if(enReg.boost)    ControlBoost(VBoostReg.ref);
    else if(enReg.current)  ControlCurrent(IBattReg.ref);
    else if(enReg.pwm)      EPWM_setCounterCompareValue(GATE_PWM_BASE, EPWM_COUNTER_COMPARE_A, (1.0-PWMVal) * TBPRD);
#else
    HandlePowerStates();
#endif

    SampleTime();

    // Display values in graph
    if(MuciEnable) {
        MUCI_Recorder_Trigger();
    }

    SampleTime();

    // Check if overflow has occurred (continuous mode)
    if(true == ADC_getInterruptOverflowStatus(ADCA_BASE, ADC_INT_NUMBER1)) {
        ADC_clearInterruptOverflowStatus(ADCA_BASE, ADC_INT_NUMBER1);
        ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
    }

    // Clear the interrupt flag
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);

    // Acknowledge the interrupt
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);

    SampleTime();
}

//
// Functions
//

// Function to toggle peripherals
void Control() {
    if(pwmState.enable) {
        pwmState.enable = 0;
        GateControl(1);
        //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);
    }

    if(pwmState.disable) {
        pwmState.disable = 0;
        GateControl(0);
        SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);
    }

    if(gateState.enable) {
        gateState.state = 1;
        gateState.enable = 0;
        GPIO_writePin(GATE_EN, 1);
    }

    if(gateState.disable) {
        gateState.state = 0;
        gateState.disable = 0;
        GPIO_writePin(GATE_EN, 0);
    }
}

// Main calculations
void Application() {
    VRef = (float)adc.Ref * 0.000805664; // Adc.Ref/4096 * 3.3

    VMainBuff[VMainBuffCnt] = VMain;
    VMainBuffCnt++;
    if(VMainBuffCnt >= 16) {
        VMainBuffCnt = 0;
    }
    VMainAvg = 0;
    int i;
    for(i = 0; i < 16; i++) {
        VMainAvg += VMainBuff[i];
    }
    VMainAvg /= 16;

    LedState();
}

//
// End of File
//
