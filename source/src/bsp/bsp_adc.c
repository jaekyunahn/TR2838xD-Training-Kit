/*
 * bsp_adc.c
 *
 *  Created on: 2024. 7. 10.
 *      Author: User
 */
#include "main.h"

Uint16 adcA2 = 0;
Uint16 adcA4 = 0;
Uint16 adcC4 = 0;
Uint16 adcD4 = 0;
Uint16 adcA13 = 0;

void init_adc(void) {
    //  start MCU Temp Sensor
    ASysCtl_enableTemperatureSensor();
    init_adca();
    init_adcc();
    init_adcd();
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
    //      Channel         : ADC_CH_ADCIN13
    //      Sample Window   : 140 SYSCLK cycles
    //      Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN13, 140U);
    ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_NONE);

    //  Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
    //      SOC number      : 0
    //      Trigger         : ADC_TRIGGER_SW_ONLY
    //      Channel         : ADC_CH_ADCIN13
    //      Sample Window   : 140 SYSCLK cycles
    //      Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN2, 15U);
    ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER1, ADC_INT_SOC_TRIGGER_NONE);

    //  Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
    //      SOC number      : 0
    //      Trigger         : ADC_TRIGGER_SW_ONLY
    //      Channel         : ADC_CH_ADCIN13
    //      Sample Window   : 140 SYSCLK cycles
    //      Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN4, 15U);
    ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER2, ADC_INT_SOC_TRIGGER_NONE);

    //  ADC Interrupt 1 Configuration
    //      Source  : ADC_SOC_NUMBER1
    //      Interrupt Source: enabled
    //      Continuous Mode : disabled
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER2);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_disableContinuousMode(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
}

void init_adcc(void) {

    //  ADC Initialization: Write ADC configurations and power up the ADC
    //  Configures the analog-to-digital converter module prescaler.
    ADC_setPrescaler(ADCC_BASE, ADC_CLK_DIV_4_0);
    //  Configures the analog-to-digital converter resolution and signal mode.
    ADC_setMode(ADCC_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
    //  Sets the timing of the end-of-conversion pulse
    ADC_setInterruptPulseMode(ADCC_BASE, ADC_PULSE_END_OF_CONV);
    //  Powers up the analog-to-digital converter core.
    ADC_enableConverter(ADCC_BASE);
    //  Delay for 1ms to allow ADC time to power up

    DEVICE_DELAY_US(10000);

    // Start of Conversion 0 Configuration

    //  Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
    //      SOC number      : 0
    //      Trigger         : ADC_TRIGGER_SW_ONLY
    //      Channel         : ADC_CH_ADCIN13
    //      Sample Window   : 15 SYSCLK cycles
    //      Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
    ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN4, 15U);
    ADC_setInterruptSOCTrigger(ADCC_BASE, ADC_SOC_NUMBER3, ADC_INT_SOC_TRIGGER_NONE);

    //  ADC Interrupt 1 Configuration
    //      Source  : ADC_SOC_NUMBER1
    //      Interrupt Source: enabled
    //      Continuous Mode : disabled
    ADC_setInterruptSource(ADCC_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER3);
    ADC_clearInterruptStatus(ADCC_BASE, ADC_INT_NUMBER1);
    ADC_disableContinuousMode(ADCC_BASE, ADC_INT_NUMBER1);
    ADC_enableInterrupt(ADCC_BASE, ADC_INT_NUMBER1);
}

void init_adcd(void) {

    //  ADC Initialization: Write ADC configurations and power up the ADC
    //  Configures the analog-to-digital converter module prescaler.
    ADC_setPrescaler(ADCD_BASE, ADC_CLK_DIV_4_0);
    //  Configures the analog-to-digital converter resolution and signal mode.
    ADC_setMode(ADCD_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
    //  Sets the timing of the end-of-conversion pulse
    ADC_setInterruptPulseMode(ADCD_BASE, ADC_PULSE_END_OF_CONV);
    //  Powers up the analog-to-digital converter core.
    ADC_enableConverter(ADCD_BASE);
    //  Delay for 1ms to allow ADC time to power up

    DEVICE_DELAY_US(10000);

    // Start of Conversion 0 Configuration

    //  Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
    //      SOC number      : 0
    //      Trigger         : ADC_TRIGGER_SW_ONLY
    //      Channel         : ADC_CH_ADCIN13
    //      Sample Window   : 15 SYSCLK cycles
    //      Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
    ADC_setupSOC(ADCD_BASE, ADC_SOC_NUMBER4, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN4, 50U);
    ADC_setInterruptSOCTrigger(ADCD_BASE, ADC_SOC_NUMBER4, ADC_INT_SOC_TRIGGER_NONE);

    //  ADC Interrupt 1 Configuration
    //      Source  : ADC_SOC_NUMBER1
    //      Interrupt Source: enabled
    //      Continuous Mode : disabled
    ADC_setInterruptSource(ADCD_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER4);
    ADC_clearInterruptStatus(ADCD_BASE, ADC_INT_NUMBER1);
    ADC_disableContinuousMode(ADCD_BASE, ADC_INT_NUMBER1);
    ADC_enableInterrupt(ADCD_BASE, ADC_INT_NUMBER1);
}

void readADC(void)
{
    //  Convert, wait for completion, and store results
    ADC_forceMultipleSOC(ADCA_BASE, ADC_FORCE_SOC0);
    //  Convert, wait for completion, and store results
    ADC_forceMultipleSOC(ADCA_BASE, ADC_FORCE_SOC1);
    //  Convert, wait for completion, and store results
    ADC_forceMultipleSOC(ADCA_BASE, ADC_FORCE_SOC2);
    //  Convert, wait for completion, and store results
    ADC_forceMultipleSOC(ADCC_BASE, ADC_FORCE_SOC3);
    //  Convert, wait for completion, and store results
    ADC_forceMultipleSOC(ADCD_BASE, ADC_FORCE_SOC4);

    //  Wait for ADCA to complete, then acknowledge flag
    while(ADC_getInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1) == false);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
    while(ADC_getInterruptStatus(ADCC_BASE, ADC_INT_NUMBER1) == false);
    ADC_clearInterruptStatus(ADCC_BASE, ADC_INT_NUMBER1);
    while(ADC_getInterruptStatus(ADCD_BASE, ADC_INT_NUMBER1) == false);
    ADC_clearInterruptStatus(ADCD_BASE, ADC_INT_NUMBER1);

    //  Read the raw result
    adcA13 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);
    //  Read the raw result
    adcA2 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER1);
    //  Read the raw result
    adcA4 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER2);
    //  Read the raw result
    adcC4 = ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER3);
    //  Read the raw result
    adcD4 = ADC_readResult(ADCDRESULT_BASE, ADC_SOC_NUMBER4);
}

Uint16 getADCval(Uint16 caseNum) {
    Uint16 res = 0;
    switch(caseNum)
    {
    case CPUTEMP:
        res = adcA13;
        break;
    case POMETER:
        res = adcA2;
        break;
    case VOLTFOLLOW:
        res = adcA4;
        break;
    case VOLTFOLLOW_LPF:
        res = adcC4;
        break;
    case VOLTFOLLOW_LPF2:
        res = adcD4;
        break;
    }
    return res;
}

//
//  End file
//
