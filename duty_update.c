/*
 * duty_update.c
 *
 *  Created on: 23-Jun-2023
 *      Author: Sanchit Awasthi
 */

#include "duty_update.h"

void duty_change (float32_t duty_a, float32_t duty_b, float32_t duty_c)
{
    int32_t d1 = (int32_t)duty_a;
    int32_t d2 = (int32_t)duty_b;
    int32_t d3 = (int32_t)duty_c;
    int16_t p1 = (int16_t)d1;
    int16_t p2 = (int16_t)d2;
    int16_t p3 = (int16_t)d3;

       EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_A, p1);
           EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_B, p1);
    EPWM_setCounterCompareValue(myEPWM2_BASE, EPWM_COUNTER_COMPARE_A, p2);
       EPWM_setCounterCompareValue(myEPWM2_BASE, EPWM_COUNTER_COMPARE_B, p2);
       EPWM_setCounterCompareValue(myEPWM3_BASE, EPWM_COUNTER_COMPARE_A,p3);
            EPWM_setCounterCompareValue(myEPWM3_BASE, EPWM_COUNTER_COMPARE_B, p3);


}


