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
    //  Read temperature (ADC)
    sensorSample = getADCval(CPUTEMP);
    //  Convert the result to a temperature in degrees C
    sensorTemp = ADC_getTemperatureC(sensorSample, 3.3f);
}

int16 getTemp(void) {
    return sensorTemp;
}


