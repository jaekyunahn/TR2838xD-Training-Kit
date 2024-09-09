/*
 * app_PWM_ADC_test.c
 *
 *  Created on: 2024. 7. 14.
 *      Author: ajg10
 */
#include "main.h"

Uint16 pometerADC = 0;
Uint16 voltageADC = 0;
Uint16 voltageLPFADC = 0;
Uint16 voltageLPFADC2 = 0;

int16 target = 0;
Uint16 pwm_val = 0;

float32 err, prop, integ = 0.0, out_err, out_sat, out, out_max = 1.0;
float32 Kp = 0.1;
float32 Ki = 1.0;

void app_pwm_adc(void) {

    struct UART_DATA uart_recive_Data;
    IIR1 Filter_Vout;
    Uint16 loop_flag = 1;

    UARTprintf("Start ADC&PWM test. Press key 'Enter' will EXIT \n");

    IIR1Init(&Filter_Vout, TWOPI * 10.0);

    //  read adc
    //  getADCval(POMETER);
    //  getADCval(VOLTFOLLOW);

    //  pwm
    //  EPwm7Regs.CMPA.bit.CMPA

    while(loop_flag) {

        //  Read POMETER
        pometerADC = getADCval(POMETER);
        //  Read Output
        //voltageADC = getADCval(VOLTFOLLOW);
        //voltageLPFADC = getADCval(VOLTFOLLOW_LPF);
        //voltageLPFADC2 = getADCval(VOLTFOLLOW_LPF2);
        voltageADC = getADCval(VOLTFOLLOW_LPF);

        voltageADC = IIR1Update(&Filter_Vout, voltageADC);

        //  Set PWM
#if 1
        err = (float32)pometerADC;
        pwm_val = (Uint16)((err / 4096.0) * (float32)period);
        pwm_val = period - pwm_val;
#else

        err = (float32)pometerADC - (float32)voltageADC;
        prop = Kp * err;
        integ = integ + Kp * (err - Ki * out_err);
        out = prop + integ;
        if(out > out_max)
            out_sat = out_max;
        else if (out < 0.0)
            out_sat = 0.0;
        else
            out_sat = out;
        out_err = out - out_sat;

        target = ((int16)(((float32)period / (float32)4096) * out)) * -1;

        if(target < 0){
            target = 0;
        }
        else if(target > 100){
            target = 100;
        }
        pwm_val = (Uint16)((float32)target * (float32)period * 0.01);
#endif
        //
        EPwm7Regs.CMPA.bit.CMPA = pwm_val;

        //
        //  pwm_val
        //  0 ~ 249 / 250
        //
        if(pwm_val >  125) app_LED_Driver(0,1); else app_LED_Driver(0,0);
        if(pwm_val >  375) app_LED_Driver(1,1); else app_LED_Driver(1,0);
        if(pwm_val >  625) app_LED_Driver(2,1); else app_LED_Driver(2,0);
        if(pwm_val >  875) app_LED_Driver(3,1); else app_LED_Driver(3,0);
        if(pwm_val > 1125) app_LED_Driver(4,1); else app_LED_Driver(4,0);
        if(pwm_val > 1375) app_LED_Driver(5,1); else app_LED_Driver(5,0);
        if(pwm_val > 1625) app_LED_Driver(6,1); else app_LED_Driver(6,0);
        if(pwm_val > 1875) app_LED_Driver(7,1); else app_LED_Driver(7,0);

        uart_recive_Data = scia_read_struct();
        //  Enter Key Press
        if(uart_recive_Data.data == 13) {
            UARTprintf("\nEnd\n");
            loop_flag = 0;
        }
    }

    //
    // Return success.
    //
    set_run_flag(0);
    return ;
}

