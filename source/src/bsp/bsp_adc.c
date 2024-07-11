/*
 * bsp_adc.c
 *
 *  Created on: 2024. 7. 10.
 *      Author: User
 */
#include "main.h"

void init_adc(void) {
    //  start MCU Temp Sensor
    ASysCtl_enableTemperatureSensor();
    init_adca();
}

//
//  ADC-A
//      SOC 0 <- ADC A13 (MCU temp Sensor)
//      Soft Trigger
//
void init_adca(void) {
    //  ADC Initialization: Write ADC configurations and power up the ADC
    //  Configures the analog-to-digital converter module prescaler.
    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_4_0);
    //  Configures the analog-to-digital converter resolution and signal mode.
    ADC_setMode(ADCA_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
    //  Sets the timing of the end-of-conversion pulse
    ADC_setInterruptPulseMode(ADCA_BASE, ADC_PULSE_END_OF_CONV);
    //  Powers up the analog-to-digital converter core.
    ADC_enableConverter(ADCA_BASE);
    //  Delay for 1ms to allow ADC time to power up

    DEVICE_DELAY_US(10000);

    // Start of Conversion 0 Configuration

    //  Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
    //      SOC number      : 0
    //      Trigger         : ADC_TRIGGER_SW_ONLY
    //      Channel         : ADC_CH_ADCIN0
    //      Sample Window   : 15 SYSCLK cycles
    //      Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN13, 140U);
    ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_NONE);

    //  ADC Interrupt 1 Configuration
    //      Source  : ADC_SOC_NUMBER1
    //      Interrupt Source: enabled
    //      Continuous Mode : disabled
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_disableContinuousMode(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
}

