/*
 * bsp_cpuTemp.c
 *
 *  Created on: 2024. 7. 13.
 *      Author: ajg10
 */
#include "main.h"

//
// Globals
//
uint16_t sensorSample;  // ADC val
int16_t sensorTemp;     // int type Temp data ('C)

void readTempSensor(void) {
    //
    //  Read temperature (ADC)
    //
    //  Convert, wait for completion, and store results
    ADC_forceMultipleSOC(ADCA_BASE, ADC_FORCE_SOC0);
    //  Wait for ADCA to complete, then acknowledge flag
    while(ADC_getInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1) == false);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
    //  Read the raw result
    sensorSample = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);
    //  Convert the result to a temperature in degrees C
    sensorTemp = ADC_getTemperatureC(sensorSample, 3.3f);
}

int16 getTemp(void) {
    return sensorTemp;
}


