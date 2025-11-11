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

#include "board.h"

//*****************************************************************************
//
// Board Configurations
// Initializes the rest of the modules. 
// Call this function in your application if you wish to do all module 
// initialization.
// If you wish to not use some of the initializations, instead of the 
// Board_init use the individual Module_inits
//
//*****************************************************************************
void Board_init()
{
	EALLOW;

	PinMux_init();
	SYNC_init();
	ASYSCTL_init();
	ADC_init();
	CMPSS_init();
	CPUTIMER_init();
	EPWM_init();
	EPWMXBAR_init();
	GPIO_init();
	INTERRUPT_init();

	EDIS;
}

//*****************************************************************************
//
// PINMUX Configurations
//
//*****************************************************************************
void PinMux_init()
{
	//
	// PinMux for modules assigned to CPU1
	//
	
	//
	// ANALOG -> myANALOGPinMux0 Pinmux
	//
	// Analog PinMux for A10/B1/C10
	GPIO_setPinConfig(GPIO_230_GPIO230);
	// AIO -> Analog mode selected
	GPIO_setAnalogMode(230, GPIO_ANALOG_ENABLED);
	// Analog PinMux for A11/B10/C0
	GPIO_setPinConfig(GPIO_237_GPIO237);
	// AIO -> Analog mode selected
	GPIO_setAnalogMode(237, GPIO_ANALOG_ENABLED);
	// Analog PinMux for A8, B0/C11
	GPIO_setPinConfig(GPIO_241_GPIO241);
	// AIO -> Analog mode selected
	GPIO_setAnalogMode(241, GPIO_ANALOG_ENABLED);
	// Analog PinMux for A9, B4/C8
	GPIO_setPinConfig(GPIO_227_GPIO227);
	// AIO -> Analog mode selected
	GPIO_setAnalogMode(227, GPIO_ANALOG_ENABLED);
	//
	// EPWM1 -> GATE_PWM Pinmux
	//
	GPIO_setPinConfig(GATE_PWM_EPWMA_PIN_CONFIG);
	GPIO_setPadConfig(GATE_PWM_EPWMA_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(GATE_PWM_EPWMA_GPIO, GPIO_QUAL_SYNC);

	GPIO_setPinConfig(GATE_PWM_EPWMB_PIN_CONFIG);
	GPIO_setPadConfig(GATE_PWM_EPWMB_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(GATE_PWM_EPWMB_GPIO, GPIO_QUAL_SYNC);

	// GPIO40 -> GATE_EN Pinmux
	GPIO_setPinConfig(GPIO_40_GPIO40);
	// GPIO6 -> PATH_STATUS Pinmux
	GPIO_setPinConfig(GPIO_6_GPIO6);
	// GPIO10 -> SYSTEM_RESET Pinmux
	GPIO_setPinConfig(GPIO_10_GPIO10);
	// GPIO4 -> LED1 Pinmux
	GPIO_setPinConfig(GPIO_4_GPIO4);
	// GPIO8 -> LED2 Pinmux
	GPIO_setPinConfig(GPIO_8_GPIO8);

}

//*****************************************************************************
//
// ADC Configurations
//
//*****************************************************************************
void ADC_init(){
	ADC0_init();
	ADC1_init();
}

void ADC0_init(){
	//
	// ADC Initialization: Write ADC configurations and power up the ADC
	//
	// Set the analog voltage reference selection and ADC module's offset trims.
	// This function sets the analog voltage reference to internal (with the reference voltage of 1.65V or 2.5V) or external for ADC
	// which is same as ASysCtl APIs.
	//
	ADC_setVREF(ADC0_BASE, ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
	//
	// Configures the analog-to-digital converter module prescaler.
	//
	ADC_setPrescaler(ADC0_BASE, ADC_CLK_DIV_2_0);
	//
	// Sets the timing of the end-of-conversion pulse
	//
	ADC_setInterruptPulseMode(ADC0_BASE, ADC_PULSE_END_OF_CONV);
	//
	// Powers up the analog-to-digital converter core.
	//
	ADC_enableConverter(ADC0_BASE);
	//
	// Delay for 1ms to allow ADC time to power up
	//
	DEVICE_DELAY_US(5000);
	//
	// SOC Configuration: Setup ADC EPWM channel and trigger settings
	//
	// Disables SOC burst mode.
	//
	ADC_disableBurstMode(ADC0_BASE);
	//
	// Sets the priority mode of the SOCs.
	//
	ADC_setSOCPriority(ADC0_BASE, ADC_PRI_SOC0_HIPRI);
	//
	// Start of Conversion 0 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 0
	//	  	Trigger			: ADC_TRIGGER_EPWM1_SOCA
	//	  	Channel			: ADC_CH_ADCIN11
	//	 	Sample Window	: 9 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(ADC0_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN11, 9U);
	ADC_setInterruptSOCTrigger(ADC0_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_NONE);
	//
	// Start of Conversion 1 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 1
	//	  	Trigger			: ADC_TRIGGER_EPWM1_SOCA
	//	  	Channel			: ADC_CH_ADCIN9
	//	 	Sample Window	: 9 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(ADC0_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN9, 9U);
	ADC_setInterruptSOCTrigger(ADC0_BASE, ADC_SOC_NUMBER1, ADC_INT_SOC_TRIGGER_NONE);
	//
	// ADC Interrupt 1 Configuration
	// 		Source	: ADC_SOC_NUMBER1
	// 		Interrupt Source: enabled
	// 		Continuous Mode	: disabled
	//
	//
	ADC_setInterruptSource(ADC0_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER1);
	ADC_clearInterruptStatus(ADC0_BASE, ADC_INT_NUMBER1);
	ADC_disableContinuousMode(ADC0_BASE, ADC_INT_NUMBER1);
	ADC_enableInterrupt(ADC0_BASE, ADC_INT_NUMBER1);
}

void ADC1_init(){
	//
	// ADC Initialization: Write ADC configurations and power up the ADC
	//
	// Set the analog voltage reference selection and ADC module's offset trims.
	// This function sets the analog voltage reference to internal (with the reference voltage of 1.65V or 2.5V) or external for ADC
	// which is same as ASysCtl APIs.
	//
	ADC_setVREF(ADC1_BASE, ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
	//
	// Configures the analog-to-digital converter module prescaler.
	//
	ADC_setPrescaler(ADC1_BASE, ADC_CLK_DIV_2_0);
	//
	// Sets the timing of the end-of-conversion pulse
	//
	ADC_setInterruptPulseMode(ADC1_BASE, ADC_PULSE_END_OF_CONV);
	//
	// Powers up the analog-to-digital converter core.
	//
	ADC_enableConverter(ADC1_BASE);
	//
	// Delay for 1ms to allow ADC time to power up
	//
	DEVICE_DELAY_US(5000);
	//
	// SOC Configuration: Setup ADC EPWM channel and trigger settings
	//
	// Disables SOC burst mode.
	//
	ADC_disableBurstMode(ADC1_BASE);
	//
	// Sets the priority mode of the SOCs.
	//
	ADC_setSOCPriority(ADC1_BASE, ADC_PRI_SOC0_HIPRI);
	//
	// Start of Conversion 0 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 0
	//	  	Trigger			: ADC_TRIGGER_EPWM1_SOCA
	//	  	Channel			: ADC_CH_ADCIN0
	//	 	Sample Window	: 9 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(ADC1_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN0, 9U);
	ADC_setInterruptSOCTrigger(ADC1_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_NONE);
	//
	// Start of Conversion 1 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 1
	//	  	Trigger			: ADC_TRIGGER_EPWM1_SOCA
	//	  	Channel			: ADC_CH_ADCIN1
	//	 	Sample Window	: 9 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(ADC1_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN1, 9U);
	ADC_setInterruptSOCTrigger(ADC1_BASE, ADC_SOC_NUMBER1, ADC_INT_SOC_TRIGGER_NONE);
}


//*****************************************************************************
//
// ASYSCTL Configurations
//
//*****************************************************************************
void ASYSCTL_init(){
	//
	// asysctl initialization
	//
	// Disables the temperature sensor output to the ADC.
	//
	ASysCtl_disableTemperatureSensor();
	//
	// Set the analog voltage reference selection to internal.
	//
	ASysCtl_setAnalogReferenceInternal( ASYSCTL_VREFHI );
	//
	// Set the internal analog voltage reference selection to 1.65V.
	//
	ASysCtl_setAnalogReference1P65( ASYSCTL_VREFHI );
}

//*****************************************************************************
//
// CMPSS Configurations
//
//*****************************************************************************
void CMPSS_init(){
	COMP_CURRENT_init();
}

void COMP_CURRENT_init(){
    //
    // Select the value for CMP1HPMXSEL.
    //
    ASysCtl_selectCMPHPMux(ASYSCTL_CMPHPMUX_SELECT_1,1U);
    //
    // Select the value for CMP1LPMXSEL.
    //
    ASysCtl_selectCMPLPMux(ASYSCTL_CMPLPMUX_SELECT_1,1U);
    //
    // Sets the configuration for the high comparator.
    //
    CMPSS_configHighComparator(COMP_CURRENT_BASE,(CMPSS_INSRC_DAC));
    //
    // Sets the configuration for the low comparator.
    //
    CMPSS_configLowComparator(COMP_CURRENT_BASE,(CMPSS_INSRC_DAC | CMPSS_INV_INVERTED));
    //
    // Sets the configuration for the internal comparator DACs.
    //
    CMPSS_configDAC(COMP_CURRENT_BASE,(CMPSS_DACVAL_SYSCLK | CMPSS_DACREF_VDDA | CMPSS_DACSRC_SHDW));
    //
    // Sets the value of the internal DAC of the high comparator.
    //
    CMPSS_setDACValueHigh(COMP_CURRENT_BASE,4034U);
    //
    // Sets the value of the internal DAC of the low comparator.
    //
    CMPSS_setDACValueLow(COMP_CURRENT_BASE,62U);
    //
    //  Configures the digital filter of the high comparator.
    //
    CMPSS_configFilterHigh(COMP_CURRENT_BASE, 0U, 9U, 5U);
    //
    // Configures the digital filter of the low comparator.
    //
    CMPSS_configFilterLow(COMP_CURRENT_BASE, 0U, 9U, 5U);
    //
    // Initializes the digital filter of the high comparator.
    //
    CMPSS_initFilterHigh(COMP_CURRENT_BASE);
    //
    // Initializes the digital filter of the low comparator.
    //
    CMPSS_initFilterLow(COMP_CURRENT_BASE);
    //
    // Sets the output signal configuration for the high comparator.
    //
    CMPSS_configOutputsHigh(COMP_CURRENT_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    //
    // Sets the output signal configuration for the low comparator.
    //
    CMPSS_configOutputsLow(COMP_CURRENT_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    //
    // Sets the comparator hysteresis settings.
    //
    CMPSS_setHysteresis(COMP_CURRENT_BASE,0U);
    //
    // Configures the comparator subsystem's ramp generator.
    //
    CMPSS_configRamp(COMP_CURRENT_BASE,0U,0U,0U,1U,true);
    //
    // Disables reset of HIGH comparator digital filter output latch on PWMSYNC
    //
    CMPSS_disableLatchResetOnPWMSYNCHigh(COMP_CURRENT_BASE);
    //
    // Disables reset of LOW comparator digital filter output latch on PWMSYNC
    //
    CMPSS_disableLatchResetOnPWMSYNCLow(COMP_CURRENT_BASE);
    //
    // Sets the ePWM module blanking signal that holds trip in reset.
    //
    CMPSS_configBlanking(COMP_CURRENT_BASE,1U);
    //
    // Disables an ePWM blanking signal from holding trip in reset.
    //
    CMPSS_disableBlanking(COMP_CURRENT_BASE);
    //
    // Configures whether or not the digital filter latches are reset by PWMSYNC
    //
    CMPSS_configLatchOnPWMSYNC(COMP_CURRENT_BASE,false,false);
    //
    // Enables the CMPSS module.
    //
    CMPSS_enableModule(COMP_CURRENT_BASE);
    //
    // Delay for CMPSS DAC to power up.
    //
    DEVICE_DELAY_US(500);
    //
    // Causes a software reset of the high comparator digital filter output latch.
    //
    CMPSS_clearFilterLatchHigh(COMP_CURRENT_BASE);
    //
    // Causes a software reset of the low comparator digital filter output latch.
    //
    CMPSS_clearFilterLatchLow(COMP_CURRENT_BASE);
}

//*****************************************************************************
//
// CPUTIMER Configurations
//
//*****************************************************************************
void CPUTIMER_init(){
	TIMER_1_init();
	TIMER_0_init();
}

void TIMER_1_init(){
	CPUTimer_setEmulationMode(TIMER_1_BASE, CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
	CPUTimer_setPreScaler(TIMER_1_BASE, 0U);
	CPUTimer_setPeriod(TIMER_1_BASE, 12000U);
	CPUTimer_enableInterrupt(TIMER_1_BASE);
	CPUTimer_stopTimer(TIMER_1_BASE);

	CPUTimer_reloadTimerCounter(TIMER_1_BASE);
	CPUTimer_startTimer(TIMER_1_BASE);
}
void TIMER_0_init(){
	CPUTimer_setEmulationMode(TIMER_0_BASE, CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
	CPUTimer_setPreScaler(TIMER_0_BASE, 0U);
	CPUTimer_setPeriod(TIMER_0_BASE, 10000U);
	CPUTimer_disableInterrupt(TIMER_0_BASE);
	CPUTimer_stopTimer(TIMER_0_BASE);

	CPUTimer_reloadTimerCounter(TIMER_0_BASE);
	CPUTimer_startTimer(TIMER_0_BASE);
}

//*****************************************************************************
//
// EPWM Configurations
//
//*****************************************************************************
void EPWM_init(){
    EPWM_setClockPrescaler(GATE_PWM_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);	
    EPWM_setTimeBasePeriod(GATE_PWM_BASE, 600);	
    EPWM_setTimeBaseCounter(GATE_PWM_BASE, 0);	
    EPWM_setTimeBaseCounterMode(GATE_PWM_BASE, EPWM_COUNTER_MODE_UP_DOWN);	
    EPWM_setCountModeAfterSync(GATE_PWM_BASE, EPWM_COUNT_MODE_UP_AFTER_SYNC);	
    EPWM_disablePhaseShiftLoad(GATE_PWM_BASE);	
    EPWM_setPhaseShift(GATE_PWM_BASE, 0);	
    EPWM_setSyncInPulseSource(GATE_PWM_BASE, EPWM_SYNC_IN_PULSE_SRC_DISABLE);	
    EPWM_setCounterCompareValue(GATE_PWM_BASE, EPWM_COUNTER_COMPARE_A, 600);	
    EPWM_setCounterCompareShadowLoadMode(GATE_PWM_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);	
    EPWM_setCounterCompareValue(GATE_PWM_BASE, EPWM_COUNTER_COMPARE_B, 0);	
    EPWM_setCounterCompareShadowLoadMode(GATE_PWM_BASE, EPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_ZERO);	
    EPWM_setActionQualifierAction(GATE_PWM_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);	
    EPWM_setActionQualifierAction(GATE_PWM_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);	
    EPWM_setActionQualifierAction(GATE_PWM_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);	
    EPWM_setActionQualifierAction(GATE_PWM_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);	
    EPWM_setActionQualifierAction(GATE_PWM_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);	
    EPWM_setActionQualifierAction(GATE_PWM_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);	
    EPWM_setActionQualifierAction(GATE_PWM_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);	
    EPWM_setActionQualifierAction(GATE_PWM_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);	
    EPWM_setActionQualifierAction(GATE_PWM_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);	
    EPWM_setActionQualifierAction(GATE_PWM_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);	
    EPWM_setActionQualifierAction(GATE_PWM_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);	
    EPWM_setActionQualifierAction(GATE_PWM_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);	
    EPWM_setDeadBandDelayPolarity(GATE_PWM_BASE, EPWM_DB_FED, EPWM_DB_POLARITY_ACTIVE_LOW);	
    EPWM_setDeadBandDelayMode(GATE_PWM_BASE, EPWM_DB_RED, true);	
    EPWM_setRisingEdgeDelayCountShadowLoadMode(GATE_PWM_BASE, EPWM_RED_LOAD_ON_CNTR_ZERO);	
    EPWM_disableRisingEdgeDelayCountShadowLoadMode(GATE_PWM_BASE);	
    EPWM_setRisingEdgeDelayCount(GATE_PWM_BASE, 12);	
    EPWM_setDeadBandDelayMode(GATE_PWM_BASE, EPWM_DB_FED, true);	
    EPWM_setFallingEdgeDelayCountShadowLoadMode(GATE_PWM_BASE, EPWM_FED_LOAD_ON_CNTR_ZERO);	
    EPWM_disableFallingEdgeDelayCountShadowLoadMode(GATE_PWM_BASE);	
    EPWM_setFallingEdgeDelayCount(GATE_PWM_BASE, 12);	
    EPWM_setTripZoneAction(GATE_PWM_BASE, EPWM_TZ_ACTION_EVENT_TZA, EPWM_TZ_ACTION_LOW);	
    EPWM_setTripZoneAction(GATE_PWM_BASE, EPWM_TZ_ACTION_EVENT_TZB, EPWM_TZ_ACTION_LOW);	
    EPWM_setTripZoneAction(GATE_PWM_BASE, EPWM_TZ_ACTION_EVENT_DCAEVT1, EPWM_TZ_ACTION_LOW);	
    EPWM_setTripZoneAction(GATE_PWM_BASE, EPWM_TZ_ACTION_EVENT_DCAEVT2, EPWM_TZ_ACTION_LOW);	
    EPWM_setTripZoneAction(GATE_PWM_BASE, EPWM_TZ_ACTION_EVENT_DCBEVT1, EPWM_TZ_ACTION_LOW);	
    EPWM_setTripZoneAction(GATE_PWM_BASE, EPWM_TZ_ACTION_EVENT_DCBEVT2, EPWM_TZ_ACTION_LOW);	
    EPWM_enableTripZoneSignals(GATE_PWM_BASE, EPWM_TZ_SIGNAL_DCAEVT1 | EPWM_TZ_SIGNAL_DCBEVT1);	
    EPWM_enableTripZoneInterrupt(GATE_PWM_BASE, EPWM_TZ_INTERRUPT_OST);	
    EPWM_selectDigitalCompareTripInput(GATE_PWM_BASE, EPWM_DC_TRIP_TRIPIN4, EPWM_DC_TYPE_DCAH);	
    EPWM_selectDigitalCompareTripInput(GATE_PWM_BASE, EPWM_DC_TRIP_TRIPIN4, EPWM_DC_TYPE_DCAL);	
    EPWM_setTripZoneDigitalCompareEventCondition(GATE_PWM_BASE, EPWM_TZ_DC_OUTPUT_A1, EPWM_TZ_EVENT_DCXH_HIGH);	
    EPWM_selectDigitalCompareTripInput(GATE_PWM_BASE, EPWM_DC_TRIP_TRIPIN4, EPWM_DC_TYPE_DCBH);	
    EPWM_selectDigitalCompareTripInput(GATE_PWM_BASE, EPWM_DC_TRIP_TRIPIN4, EPWM_DC_TYPE_DCBL);	
    EPWM_setTripZoneDigitalCompareEventCondition(GATE_PWM_BASE, EPWM_TZ_DC_OUTPUT_B1, EPWM_TZ_EVENT_DCXL_HIGH);	
    EPWM_enableADCTrigger(GATE_PWM_BASE, EPWM_SOC_A);	
    EPWM_setADCTriggerSource(GATE_PWM_BASE, EPWM_SOC_A, EPWM_SOC_TBCTR_ZERO_OR_PERIOD);	
    EPWM_setADCTriggerEventPrescale(GATE_PWM_BASE, EPWM_SOC_A, 1);	
    EPWM_enableADCTriggerEventCountInit(GATE_PWM_BASE, EPWM_SOC_A);	
}

//*****************************************************************************
//
// EPWMXBAR Configurations
//
//*****************************************************************************
void EPWMXBAR_init(){
	myEPWMXBAR0_init();
}

void myEPWMXBAR0_init(){
		
	XBAR_setEPWMMuxConfig(myEPWMXBAR0, XBAR_EPWM_MUX00_CMPSS1_CTRIPH_OR_L);
	XBAR_enableEPWMMux(myEPWMXBAR0, XBAR_MUX00);
}

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
void GPIO_init(){
	GATE_EN_init();
	PATH_STATUS_init();
	SYSTEM_RESET_init();
	LED1_init();
	LED2_init();
}

void GATE_EN_init(){
	GPIO_writePin(GATE_EN, 0);
	GPIO_setPadConfig(GATE_EN, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(GATE_EN, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(GATE_EN, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(GATE_EN, GPIO_CORE_CPU1);
}
void PATH_STATUS_init(){
	GPIO_setPadConfig(PATH_STATUS, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(PATH_STATUS, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(PATH_STATUS, GPIO_DIR_MODE_IN);
	GPIO_setControllerCore(PATH_STATUS, GPIO_CORE_CPU1);
}
void SYSTEM_RESET_init(){
	GPIO_writePin(SYSTEM_RESET, 0);
	GPIO_setPadConfig(SYSTEM_RESET, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(SYSTEM_RESET, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(SYSTEM_RESET, GPIO_DIR_MODE_IN);
	GPIO_setControllerCore(SYSTEM_RESET, GPIO_CORE_CPU1);
}
void LED1_init(){
	GPIO_writePin(LED1, 0);
	GPIO_setPadConfig(LED1, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(LED1, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(LED1, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(LED1, GPIO_CORE_CPU1);
}
void LED2_init(){
	GPIO_writePin(LED2, 0);
	GPIO_setPadConfig(LED2, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(LED2, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(LED2, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(LED2, GPIO_CORE_CPU1);
}

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************
void INTERRUPT_init(){
	
	// Interrupt Settings for INT_ADC0_1
	// ISR need to be defined for the registered interrupts
	Interrupt_register(INT_ADC0_1, &INT_ADC0_1_ISR);
	Interrupt_enable(INT_ADC0_1);
	
	// Interrupt Settings for INT_TIMER_1
	// ISR need to be defined for the registered interrupts
	Interrupt_register(INT_TIMER_1, &INT_TIMER_1_ISR);
	Interrupt_enable(INT_TIMER_1);
}
//*****************************************************************************
//
// SYNC Scheme Configurations
//
//*****************************************************************************
void SYNC_init(){
	SysCtl_setSyncOutputConfig(SYSCTL_SYNC_OUT_SRC_EPWM1SYNCOUT);
	//
	// SOCA
	//
	SysCtl_enableExtADCSOCSource(0);
	//
	// SOCB
	//
	SysCtl_enableExtADCSOCSource(0);
}
