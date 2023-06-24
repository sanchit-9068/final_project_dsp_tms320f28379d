/*
 * pwm.c
 *
 *  Created on: 13-Jun-2023
 *      Author: Sanchit Awasthi
 */

#include "pwm.h"

#define EPWM1_TIMER_TBPRD  5000U


#define EPWM2_TIMER_TBPRD  5000U


#define EPWM3_TIMER_TBPRD  5000U


#define EPWM_CMP_UP           1U
#define EPWM_CMP_DOWN         0U
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

    PinMux_init_PWM();
    SYNC_init();
    EPWM_init();

    EDIS;
}
uint16_t sector_vector = 3;
   float32_t u2;
   float32_t x,y,z,t1,t2,t7;float32_t result1,result2,result3;
void variable_def(float32_t ualpha, float32_t ubeta)
{
    u2 = 0.866f * ualpha;
    if ( ubeta >=0 && ubeta <(u2))
                sector_vector = 1;
            else if (((u2) > 0 && ubeta > (u2))||(u2 < 0 && (ubeta > (-1.00f*u2))))
                sector_vector = 2;
            else if( ubeta > 0 && (-1.00f*u2)>=ubeta)
                sector_vector = 3;
            else if( ubeta <=0 && u2 < ubeta)
                sector_vector = 4;
            else if ( ubeta < 0 && (-1.00f*u2) <= ubeta)
                sector_vector = 6;
            else
                sector_vector = 5;


            x = ubeta;
            y = u2 + 0.5*ubeta;
            z = -1.00f*u2 + 0.5f*ubeta;

            if (sector_vector == 1)
            {
                t1 = -1.00f*z;
                t2 = x;
                t7 = (1-t1-t2)/2.00f;
                result1 = t1 + t2 + t7;
                result2 = t2+t7;
                result3 = t7;
            }
            else if (sector_vector == 2)
            {
                t1 = z;
                t2 = y;
                t7 = (1-t1-t2)/2.00f;
                result1 = t1 + t7;
                result2 = t1 + t2 + t7;
                result3 = t7;

            }
            else if (sector_vector == 3)
            {
                t1 = x;
                t2 = y;
                t7 = (1-t1-t2)/2.00f;
                result1 = t7;
                result2 = t1 + t2 + t7;
                result3 = t2 + t7;

            }
            else if (sector_vector == 4)
            {
                t1 = -1.00f*x;
                t2 = z;
                t7 = (1-t1-t2)/2.00f;
                result1 = t7;
                result2 = t1 + t7;
                result3 = t1 + t2 + t7;
            }
            else if (sector_vector == 5)
            {
                t1 = -1.00f*y;
                t2 = -1.00f*z;
                t7 = (1-t1-t2)/2.00f;
                result1 = t2 + t7;
                result2 = t7;
                result3 = t1 + t2 + t7;
            }
            else
            {
                t1 = y;
                t2 = -1.00f*x;
                t7 = (1-t1-t2)/2.00f;
                result1 = t1 + t2 + t7;
                result2 = t7;
                result3 = t1 + t7;

            }
}

//*****************************************************************************
//
// PINMUX Configurations
//
//*****************************************************************************
void PinMux_init_PWM()
{
    //
    // PinMux for modules assigned to CPU1
    //

    //
    // EPWM1 -> myEPWM1 Pinmux
    //
    GPIO_setPinConfig(myEPWM1_EPWMA_PIN_CONFIG);
    GPIO_setPadConfig(myEPWM1_EPWMA_GPIO, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(myEPWM1_EPWMA_GPIO, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(myEPWM1_EPWMB_PIN_CONFIG);
    GPIO_setPadConfig(myEPWM1_EPWMB_GPIO, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(myEPWM1_EPWMB_GPIO, GPIO_QUAL_SYNC);

    //
    // EPWM2 -> myEPWM2 Pinmux
    //
    GPIO_setPinConfig(myEPWM2_EPWMA_PIN_CONFIG);
    GPIO_setPadConfig(myEPWM2_EPWMA_GPIO, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(myEPWM2_EPWMA_GPIO, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(myEPWM2_EPWMB_PIN_CONFIG);
    GPIO_setPadConfig(myEPWM2_EPWMB_GPIO, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(myEPWM2_EPWMB_GPIO, GPIO_QUAL_SYNC);

    //
    // EPWM3 -> myEPWM3 Pinmux
    //
    GPIO_setPinConfig(myEPWM3_EPWMA_PIN_CONFIG);
    GPIO_setPadConfig(myEPWM3_EPWMA_GPIO, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(myEPWM3_EPWMA_GPIO, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(myEPWM3_EPWMB_PIN_CONFIG);
    GPIO_setPadConfig(myEPWM3_EPWMB_GPIO, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(myEPWM3_EPWMB_GPIO, GPIO_QUAL_SYNC);


}

//*****************************************************************************
//
// EPWM Configurations
//
//*****************************************************************************
void EPWM_init(){
    EPWM_setClockPrescaler(myEPWM1_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_2);
    EPWM_setTimeBasePeriod(myEPWM1_BASE, 0);
    EPWM_setTimeBaseCounter(myEPWM1_BASE, 0);
    EPWM_setTimeBaseCounterMode(myEPWM1_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);
    EPWM_disablePhaseShiftLoad(myEPWM1_BASE);
    EPWM_setPhaseShift(myEPWM1_BASE, 0);
    EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_A, 0);
    EPWM_setCounterCompareShadowLoadMode(myEPWM1_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_B, 0);
    EPWM_setCounterCompareShadowLoadMode(myEPWM1_BASE, EPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);
    EPWM_setClockPrescaler(myEPWM2_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_2);
    EPWM_setTimeBasePeriod(myEPWM2_BASE, 0);
    EPWM_setTimeBaseCounter(myEPWM2_BASE, 0);
    EPWM_setTimeBaseCounterMode(myEPWM2_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);
    EPWM_disablePhaseShiftLoad(myEPWM2_BASE);
    EPWM_setPhaseShift(myEPWM2_BASE, 0);
    EPWM_setCounterCompareValue(myEPWM2_BASE, EPWM_COUNTER_COMPARE_A, 0);
    EPWM_setCounterCompareShadowLoadMode(myEPWM2_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setCounterCompareValue(myEPWM2_BASE, EPWM_COUNTER_COMPARE_B, 0);
    EPWM_setCounterCompareShadowLoadMode(myEPWM2_BASE, EPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);
    EPWM_setClockPrescaler(myEPWM3_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_2);
    EPWM_setTimeBasePeriod(myEPWM3_BASE, 0);
    EPWM_setTimeBaseCounter(myEPWM3_BASE, 0);
    EPWM_setTimeBaseCounterMode(myEPWM3_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);
    EPWM_disablePhaseShiftLoad(myEPWM3_BASE);
    EPWM_setPhaseShift(myEPWM3_BASE, 0);
    EPWM_setCounterCompareValue(myEPWM3_BASE, EPWM_COUNTER_COMPARE_A, 0);
    EPWM_setCounterCompareShadowLoadMode(myEPWM3_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setCounterCompareValue(myEPWM3_BASE, EPWM_COUNTER_COMPARE_B, 0);
    EPWM_setCounterCompareShadowLoadMode(myEPWM3_BASE, EPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);
}

//*****************************************************************************
//
// SYNC Scheme Configurations
//
//*****************************************************************************
void SYNC_init(){
    SysCtl_setSyncOutputConfig(SYSCTL_SYNC_OUT_SRC_EPWM1SYNCOUT);
    //
    // For EPWM1, the sync input is: SYSCTL_SYNC_IN_SRC_EXTSYNCIN1
    //
    SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_EPWM4, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
    SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_EPWM7, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
    SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_EPWM10, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
    SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_ECAP1, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
    SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_ECAP4, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
    //
    // SOCA
    //
    SysCtl_enableExtADCSOCSource(0);
    //
    // SOCB
    //
    SysCtl_enableExtADCSOCSource(0);
}


float32_t ta_pwm2( )
{





          return result1;


}


float32_t tb_pwm2()
{


       return result2;



}



float32_t tc_pwm2( )
{



             return result3;



}



void initEPWM1()
{
   //
   // Set-up TBCLK
   //
   EPWM_setTimeBasePeriod(myEPWM1_BASE, EPWM1_TIMER_TBPRD);
//   EPWM_setPhaseShift(myEPWM1_BASE, 0U);
   EPWM_setTimeBaseCounter(myEPWM1_BASE, 0U);
   //
   // Set Compare values
   //
   EPWM_setCounterCompareValue(myEPWM1_BASE,
                               EPWM_COUNTER_COMPARE_A,
                            0);
   EPWM_setCounterCompareValue(myEPWM1_BASE,
                               EPWM_COUNTER_COMPARE_B,
                               0);

   //
   // Set up counter mode
   //
   EPWM_setTimeBaseCounterMode(myEPWM1_BASE, EPWM_COUNTER_MODE_UP_DOWN);
//   EPWM_enablePhaseShiftLoad(myEPWM1_BASE);
   EPWM_setClockPrescaler(myEPWM1_BASE,
                          EPWM_CLOCK_DIVIDER_1,
                          EPWM_HSCLOCK_DIVIDER_1);

   //
   // Set up shadowing
   //
   EPWM_setCounterCompareShadowLoadMode(myEPWM1_BASE,
                                        EPWM_COUNTER_COMPARE_A,
                                        EPWM_COMP_LOAD_ON_CNTR_ZERO);
   EPWM_setCounterCompareShadowLoadMode(myEPWM1_BASE,
                                        EPWM_COUNTER_COMPARE_B,
                                        EPWM_COMP_LOAD_ON_CNTR_ZERO);

   //
   // Set actions
   //
   EPWM_setActionQualifierAction(myEPWM1_BASE,
                                 EPWM_AQ_OUTPUT_A,
                                 EPWM_AQ_OUTPUT_HIGH,
                                 EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
   EPWM_setActionQualifierAction(myEPWM1_BASE,
                                 EPWM_AQ_OUTPUT_A,
                                 EPWM_AQ_OUTPUT_LOW,
                                 EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);
   EPWM_setActionQualifierAction(myEPWM1_BASE,
                                 EPWM_AQ_OUTPUT_B,
                                 EPWM_AQ_OUTPUT_LOW,
                                 EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
   EPWM_setActionQualifierAction(myEPWM1_BASE,
                                 EPWM_AQ_OUTPUT_B,
                                 EPWM_AQ_OUTPUT_HIGH,
                                 EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);

}

//
// initEPWM2 - Configure ePWM2
//
void initEPWM2()
{
   //
   // Set-up TBCLK
   //
   EPWM_setTimeBasePeriod(myEPWM2_BASE, EPWM2_TIMER_TBPRD);
//   EPWM_setPhaseShift(myEPWM2_BASE, 0U);
   EPWM_setTimeBaseCounter(myEPWM2_BASE, 0U);

   //
   // Set Compare values
   //
   EPWM_setCounterCompareValue(myEPWM2_BASE,
                               EPWM_COUNTER_COMPARE_A,
                               0);
   EPWM_setCounterCompareValue(myEPWM2_BASE,
                               EPWM_COUNTER_COMPARE_B,
                               0);

   //
   // Set-up counter mode
   //
   EPWM_setTimeBaseCounterMode(myEPWM2_BASE, EPWM_COUNTER_MODE_UP_DOWN);
//   EPWM_enablePhaseShiftLoad(myEPWM2_BASE);
   EPWM_setClockPrescaler(myEPWM2_BASE,
                          EPWM_CLOCK_DIVIDER_1,
                          EPWM_HSCLOCK_DIVIDER_1);

   //
   // Set-up shadowing
   //
   EPWM_setCounterCompareShadowLoadMode(myEPWM2_BASE,
                                        EPWM_COUNTER_COMPARE_A,
                                        EPWM_COMP_LOAD_ON_CNTR_ZERO);
   EPWM_setCounterCompareShadowLoadMode(myEPWM2_BASE,
                                        EPWM_COUNTER_COMPARE_B,
                                        EPWM_COMP_LOAD_ON_CNTR_ZERO);

   //
   // Set Action qualifier
   //
   EPWM_setActionQualifierAction(myEPWM2_BASE,
                                 EPWM_AQ_OUTPUT_A,
                                 EPWM_AQ_OUTPUT_HIGH,
                                 EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
   EPWM_setActionQualifierAction(myEPWM2_BASE,
                                 EPWM_AQ_OUTPUT_A,
                                 EPWM_AQ_OUTPUT_LOW,
                                 EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);
   EPWM_setActionQualifierAction(myEPWM2_BASE,
                                 EPWM_AQ_OUTPUT_B,
                                 EPWM_AQ_OUTPUT_LOW,
                                 EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
   EPWM_setActionQualifierAction(myEPWM2_BASE,
                                 EPWM_AQ_OUTPUT_B,
                                 EPWM_AQ_OUTPUT_HIGH,
                                 EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);

}
//
// initEPWM3 - Configure ePWM3
//
void initEPWM3(void)
{
   //
      // Set-up TBCLK
      //
      EPWM_setTimeBasePeriod(myEPWM3_BASE, EPWM2_TIMER_TBPRD);
//      EPWM_setPhaseShift(myEPWM3_BASE, 0U);
      EPWM_setTimeBaseCounter(myEPWM3_BASE, 0U);

      //
      // Set Compare values
      //
      EPWM_setCounterCompareValue(myEPWM3_BASE,
                                  EPWM_COUNTER_COMPARE_A,
                                  0);
      EPWM_setCounterCompareValue(myEPWM3_BASE,
                                  EPWM_COUNTER_COMPARE_B,
                                  0);

      //
      // Set-up counter mode
      //
      EPWM_setTimeBaseCounterMode(myEPWM3_BASE, EPWM_COUNTER_MODE_UP_DOWN);
//      EPWM_enablePhaseShiftLoad(myEPWM3_BASE);
      EPWM_setClockPrescaler(myEPWM3_BASE,
                             EPWM_CLOCK_DIVIDER_1,
                             EPWM_HSCLOCK_DIVIDER_1);

      //
      // Set-up shadowing
      //
      EPWM_setCounterCompareShadowLoadMode(myEPWM3_BASE,
                                           EPWM_COUNTER_COMPARE_A,
                                           EPWM_COMP_LOAD_ON_CNTR_ZERO);
      EPWM_setCounterCompareShadowLoadMode(myEPWM3_BASE,
                                           EPWM_COUNTER_COMPARE_B,
                                           EPWM_COMP_LOAD_ON_CNTR_ZERO);

      //
      // Set Action qualifier
      //
      EPWM_setActionQualifierAction(myEPWM3_BASE,
                                    EPWM_AQ_OUTPUT_A,
                                    EPWM_AQ_OUTPUT_HIGH,
                                    EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
      EPWM_setActionQualifierAction(myEPWM3_BASE,
                                    EPWM_AQ_OUTPUT_A,
                                    EPWM_AQ_OUTPUT_LOW,
                                    EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);
      EPWM_setActionQualifierAction(myEPWM3_BASE,
                                    EPWM_AQ_OUTPUT_B,
                                    EPWM_AQ_OUTPUT_LOW,
                                    EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
      EPWM_setActionQualifierAction(myEPWM3_BASE,
                                    EPWM_AQ_OUTPUT_B,
                                    EPWM_AQ_OUTPUT_HIGH,
                                    EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);

}

