/*
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef BOARD_H
#define BOARD_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//
// Included Files
//

#include "driverlib.h"
#include "device.h"

//*****************************************************************************
//
// PinMux Configurations
//
//*****************************************************************************

//
// ANALOG -> myANALOGPinMux0 Pinmux
//

//
// EPWM1 -> GATE_PWM Pinmux
//
//
// EPWM1_A - GPIO Settings
//
#define GPIO_PIN_EPWM1_A 0
#define GATE_PWM_EPWMA_GPIO 0
#define GATE_PWM_EPWMA_PIN_CONFIG GPIO_0_EPWM1_A
//
// EPWM1_B - GPIO Settings
//
#define GPIO_PIN_EPWM1_B 1
#define GATE_PWM_EPWMB_GPIO 1
#define GATE_PWM_EPWMB_PIN_CONFIG GPIO_1_EPWM1_B
//
// GPIO40 - GPIO Settings
//
#define GATE_EN_GPIO_PIN_CONFIG GPIO_40_GPIO40
//
// GPIO6 - GPIO Settings
//
#define PATH_STATUS_GPIO_PIN_CONFIG GPIO_6_GPIO6
//
// GPIO10 - GPIO Settings
//
#define SYSTEM_RESET_GPIO_PIN_CONFIG GPIO_10_GPIO10
//
// GPIO4 - GPIO Settings
//
#define LED1_GPIO_PIN_CONFIG GPIO_4_GPIO4
//
// GPIO8 - GPIO Settings
//
#define LED2_GPIO_PIN_CONFIG GPIO_8_GPIO8

//*****************************************************************************
//
// ADC Configurations
//
//*****************************************************************************
#define ADC0_BASE ADCA_BASE
#define ADC0_RESULT_BASE ADCARESULT_BASE
#define ADC0_SOC0 ADC_SOC_NUMBER0
#define ADC0_FORCE_SOC0 ADC_FORCE_SOC0
#define ADC0_SAMPLE_WINDOW_SOC0 75
#define ADC0_TRIGGER_SOURCE_SOC0 ADC_TRIGGER_EPWM1_SOCA
#define ADC0_CHANNEL_SOC0 ADC_CH_ADCIN11
#define ADC0_SOC1 ADC_SOC_NUMBER1
#define ADC0_FORCE_SOC1 ADC_FORCE_SOC1
#define ADC0_SAMPLE_WINDOW_SOC1 75
#define ADC0_TRIGGER_SOURCE_SOC1 ADC_TRIGGER_EPWM1_SOCA
#define ADC0_CHANNEL_SOC1 ADC_CH_ADCIN9
void ADC0_init();

#define ADC1_BASE ADCB_BASE
#define ADC1_RESULT_BASE ADCBRESULT_BASE
#define ADC1_SOC0 ADC_SOC_NUMBER0
#define ADC1_FORCE_SOC0 ADC_FORCE_SOC0
#define ADC1_SAMPLE_WINDOW_SOC0 75
#define ADC1_TRIGGER_SOURCE_SOC0 ADC_TRIGGER_EPWM1_SOCA
#define ADC1_CHANNEL_SOC0 ADC_CH_ADCIN0
#define ADC1_SOC1 ADC_SOC_NUMBER1
#define ADC1_FORCE_SOC1 ADC_FORCE_SOC1
#define ADC1_SAMPLE_WINDOW_SOC1 75
#define ADC1_TRIGGER_SOURCE_SOC1 ADC_TRIGGER_EPWM1_SOCA
#define ADC1_CHANNEL_SOC1 ADC_CH_ADCIN1
void ADC1_init();


//*****************************************************************************
//
// ASYSCTL Configurations
//
//*****************************************************************************

//*****************************************************************************
//
// CMPSS Configurations
//
//*****************************************************************************
#define COMP_CURRENT_BASE CMPSS1_BASE
#define COMP_CURRENT_HIGH_BASE CMPSS1_BASE    
#define COMP_CURRENT_LOW_BASE CMPSS1_BASE    
void COMP_CURRENT_init();

//*****************************************************************************
//
// CPUTIMER Configurations
//
//*****************************************************************************
#define TIMER_1MS_BASE CPUTIMER1_BASE
void TIMER_1MS_init();

//*****************************************************************************
//
// EPWM Configurations
//
//*****************************************************************************
#define GATE_PWM_BASE EPWM1_BASE
#define GATE_PWM_TBPRD 600
#define GATE_PWM_COUNTER_MODE EPWM_COUNTER_MODE_UP_DOWN
#define GATE_PWM_TBPHS 0
#define GATE_PWM_CMPA 600
#define GATE_PWM_CMPB 0
#define GATE_PWM_CMPC 0
#define GATE_PWM_CMPD 0
#define GATE_PWM_DBRED 12
#define GATE_PWM_DBFED 12
#define GATE_PWM_TZA_ACTION EPWM_TZ_ACTION_LOW
#define GATE_PWM_TZB_ACTION EPWM_TZ_ACTION_LOW
#define GATE_PWM_OSHT_SOURCES (EPWM_TZ_SIGNAL_DCAEVT1 | EPWM_TZ_SIGNAL_DCBEVT1)
#define GATE_PWM_TZ_INTERRUPT_SOURCES EPWM_TZ_INTERRUPT_OST
#define GATE_PWM_INTERRUPT_SOURCE EPWM_INT_TBCTR_DISABLED

//*****************************************************************************
//
// EPWMXBAR Configurations
//
//*****************************************************************************
void myEPWMXBAR0_init();
#define myEPWMXBAR0 XBAR_TRIP4
#define myEPWMXBAR0_ENABLED_MUXES (XBAR_MUX00)

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
#define GATE_EN 40
void GATE_EN_init();
#define PATH_STATUS 6
void PATH_STATUS_init();
#define SYSTEM_RESET 10
void SYSTEM_RESET_init();
#define LED1 4
void LED1_init();
#define LED2 8
void LED2_init();

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_ADC0_1
// ISR need to be defined for the registered interrupts
#define INT_ADC0_1 INT_ADCA1
#define INT_ADC0_1_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP1
extern __interrupt void INT_ADC0_1_ISR(void);

// Interrupt Settings for INT_TIMER_1MS
// ISR need to be defined for the registered interrupts
#define INT_TIMER_1MS INT_TIMER1
extern __interrupt void INT_TIMER_1MS_ISR(void);

// Interrupt Settings for INT_GATE_PWM_TZ
// ISR need to be defined for the registered interrupts
#define INT_GATE_PWM_TZ INT_EPWM1_TZ
#define INT_GATE_PWM_TZ_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP2
extern __interrupt void INT_GATE_PWM_TZ_ISR(void);

//*****************************************************************************
//
// SYNC Scheme Configurations
//
//*****************************************************************************

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	ADC_init();
void	ASYSCTL_init();
void	CMPSS_init();
void	CPUTIMER_init();
void	EPWM_init();
void	EPWMXBAR_init();
void	GPIO_init();
void	INTERRUPT_init();
void	SYNC_init();
void	PinMux_init();

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif  // end of BOARD_H definition
