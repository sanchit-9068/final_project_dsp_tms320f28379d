/*
 * Master source file
 *
 *  Created on: 13-Jun-2023
 *      Author: Sanchit Awasthi
 */
//
// Included Files
//
#define _USE_MATH_DEFINES
#include <encoder.h>
#include "driverlib.h"
#include "device.h"
#include "sci_init.h"
#include "cpu_timer.h"
#include "clark.h"
#include "IQmathLib.h"
#include "math.h"
#include "park.h"
#include "pid.h"
#include "inverse_park.h"
#include "pwm.h"
#include "duty_update.h"
//
// Main
//
#define  PWM_FREQUENCY           10.0
#define SYSTEM_FREQUENCY 200
#define  INV_PWM_TICKS              ((SYSTEM_FREQUENCY/2.0)/PWM_FREQUENCY)*1000
#define  QEP_UNIT_TIMER_TICKS       ((SYSTEM_FREQUENCY/(2*PWM_FREQUENCY))*1000)
#define  INV_PWM_TBPRD              INV_PWM_TICKS/2
#define  INV_PWM_HALF_TBPRD         INV_PWM_TICKS/4
#define Rs 7.88 //in ohms
#define Ld 0.0344 //in mH
#define J  0.000605 //in kg/m2
#define Kpc 5.00*Rs
#define Kic 25.00*Rs*Rs/Ld
#define Kps 0.5*Rs*J/Ld;
#define Kis 0.25*Rs*Rs*J/(Ld*Ld);
#define MaxCurrent 10.00 //in amps
#define MinCurrent -10.00// in amps
#define MaxVq 300 //in rpm
#define MinVq -300//in rpm


float32_t pos_float=0.0f;

#define     EX_ADC_RESOLUTION       12

void ConfigADC(uint32_t ADC_BASE);
void initADC_SOC(void);

uint16_t Adc_Result_1,Adc_Result_2,Adc_Result_3;
float ADCINA0_1A,ADCINA4_1B,ADCINA2_1C;



__interrupt void cpuTimer0ISR(void);
__interrupt void cpuTimer1ISR(void);
uint16_t cpuTimer0IntCount;

int32_t prev_pos=0;

float32_t ialpha,ibeta,id,iq;
float32_t ia=2.00,ib=3.00;
int tester=0;
float32_t speed_motor;


float32_t duty_a,duty_b,duty_c;




// Function to calculate the speed.
float calculateSpeed()
{
    uint32_t position = EQEP_getPosition(EQEP1_BASE);
    float deltaTime = 0.001f;  // Time interval between speed calculations (in seconds)
    int32_t y=position-prev_pos;
    // Calculate speed in encoder counts per second
    if(y<0)
        y*=-1;

    float speedCountsPerSec = (float)(y) / deltaTime;
    prev_pos=position;
    // Calculate speed in RPM
    float speedRPM = (speedCountsPerSec /( ENCODER_RESOLUTION*4)) * 60.0f;

    return speedRPM;
}

PIDController pid_current_q;
PIDController pid_speed;
PIDController pid_current_d;

float32_t iqref=10.0;   //Give the iqref current value here
float32_t vq =0.0;
float32_t idref=5.0;    //Give the idref current value here
float32_t vd=0.0;


float32_t id_measured=0.0;
float32_t iq_measured=0.0;
float32_t Ta,Tb,Tc;
float32_t valpha_inverse,vbeta_inverse;
uint16_t seca,secb,secc;

void main(void)
{
    // Initialize system and EQEP module
        // ...
        Device_init();

            //
            // Disable pin locks and enable internal pullups.
            //
        Device_initGPIO();
        Pin_EQEP();
        setupEQEPModule();
        Interrupt_initModule();
        PinMux_init();
        mySCI0_init();
        Board_init();

        SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
        initEPWM1();
        initEPWM2();
        initEPWM3();
        SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

        //
        // Initialize the PIE vector table with pointers to the shell Interrupt
        // Service Routines (ISR).
        //
        Interrupt_initVectorTable();


        Interrupt_register(INT_TIMER0, &cpuTimer0ISR);
        Interrupt_register(INT_TIMER1, &cpuTimer1ISR);
        initCPUTimers();
        configCPUTimer(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ, 1000);// interrupt every 1 millisecond
        configCPUTimer(CPUTIMER1_BASE, DEVICE_SYSCLK_FREQ, 1000);// interrupt every 1 millisecond
        CPUTimer_enableInterrupt(CPUTIMER0_BASE);
        CPUTimer_enableInterrupt(CPUTIMER1_BASE);
        Interrupt_enable(INT_TIMER0);
        Interrupt_enable(INT_TIMER1);
        CPUTimer_startTimer(CPUTIMER0_BASE);
        CPUTimer_startTimer(CPUTIMER1_BASE);


        ConfigADC(ADCA_BASE);
        initADC_SOC();


        EINT;
        ERTM;



        pid_current_q.Kd = 0.0;
        pid_current_q.Ki = Kic;
        pid_current_q.Kp = Kpc;
        pid_current_q.limMax = MaxVq;
        pid_current_q.limMin = MinVq;
        pid_current_q.limMaxInt = MaxVq/2.00;
        pid_current_q.limMinInt = MinVq/2.00;
        PIDController_Init(&pid_current_q);



//        pid_current_d.Kd = 0.0;
//        pid_current_d.Ki = Kic;
//        pid_current_d.Kp = Kpc;
//        pid_current_d.limMax = MaxVq;
//        pid_current_d.limMin = MinVq;
//        pid_current_d.limMaxInt = MaxVq/2.00;
//        pid_current_d.limMinInt = MinVq/2.00;
//        PIDController_Init(&pid_current_d);




        pid_speed.Kd = 0.0;
        pid_speed.Ki = Kis;
        pid_speed.Kp = Kps;
        pid_speed.limMax = MaxCurrent;
        pid_speed.limMin = MinCurrent;
        pid_speed.limMaxInt = MaxCurrent/2.00;
        pid_speed.limMinInt = MinCurrent/2.00;
        PIDController_Init(&pid_speed);




        while (1)
        {

            ADC_forceSOC(ADCA_BASE, ADC_SOC_NUMBER0);
              ADC_forceSOC(ADCA_BASE, ADC_SOC_NUMBER2);
              ADC_forceSOC(ADCA_BASE, ADC_SOC_NUMBER4);

              while(ADC_getInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1) == false)
                  {

                  }
              ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);

              ////           // Store results
              Adc_Result_1 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);
              Adc_Result_2 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER4);
              Adc_Result_3 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER2);

              // convert into voltage

   //           ADCINA0_1A = Adc_Result_1*(3.3/4096);
   //           ADCINA4_1B = Adc_Result_2*(3.3/4096);
   //           ADCINA2_1C = Adc_Result_3*(3.3/4096);

              // convert these voltage levels into AMP

              ADCINA0_1A = (Adc_Result_1*(3.3/4096))*100;
             ADCINA4_1B = (Adc_Result_2*(3.3/4096))*100;
             ADCINA2_1C = (Adc_Result_3*(3.3/4096))*100;
             ia = ADCINA0_1A;
             ib = ADCINA4_1B;

            ialpha=clark_alpha(ia, ib);
            ibeta=clark_beta(ia, ib);
            id_measured= park_id(ialpha, ibeta, pos_float);
            iq_measured= park_iq(ialpha, ibeta, pos_float);
            valpha_inverse = inverse_park_alpha(vd, vq, pos_float);
            vbeta_inverse = inverse_park_beta(vd, vq, pos_float);


            variable_def(valpha_inverse, vbeta_inverse);
            Ta = ta_pwm2();
            Tb = tb_pwm2();
            Tc = tc_pwm2();

            //        duty_a = Ta*INV_PWM_HALF_TBPRD + INV_PWM_HALF_TBPRD;
            //        duty_b = Tb*INV_PWM_HALF_TBPRD + INV_PWM_HALF_TBPRD;
            //        duty_c = Tc*INV_PWM_HALF_TBPRD + INV_PWM_HALF_TBPRD;

            duty_a = Ta * 5000;
            duty_b = Tb * 5000;
            duty_c = Tc * 5000;
            duty_change(duty_a, duty_b, duty_c);

        }



        return 0;
}


__interrupt void
 cpuTimer0ISR(void)
 {


//            PIDController_Update(&pid_speed, 500, speed_motor);
//            iqref = pid_speed.output;
            PIDController_Update(&pid_current_q, iqref, iq_measured);
            vq = pid_current_q.output;
            PIDController_Update(&pid_current_d, idref, id_measured);
            vd = pid_current_d.output;
     //
     // Acknowledge this interrupt to receive more interrupts from group 1
     //
     Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
 }



__interrupt void
 cpuTimer1ISR(void)
 {

    cpuTimer0IntCount++;
        float speedRPM = calculateSpeed()/4.00f;
        speed_motor = speedRPM;
               uint32_t j=(int32_t)speedRPM;
               uint16_t m=(int16_t)j;
              char l[16]={};

               SCI_writeCharArray(mySCI0_BASE, (uint16_t *)("SPEED="), sizeof("SPEED="));
               intToStrPositive(m, l);
               SCI_writeCharArray(mySCI0_BASE, (uint16_t *)l, sizeof(l));
               SCI_writeCharArray(mySCI0_BASE, (uint16_t *)("\r\n"), sizeof("\r\n"));
               int32_t pos=EQEP_getPosition(EQEP1_BASE);
             pos%=4096;
             pos_float = (pos*360.00f)/4096.00f;
             int32_t pos2=(int32_t)pos_float;
             int16_t angle=(int16_t)pos2;
             char pos_char[16]={};
             SCI_writeCharArray(mySCI0_BASE, (uint16_t *)("Angle="), sizeof("Angle="));
             intToStrPositive(angle, pos_char);
             SCI_writeCharArray(mySCI0_BASE, (uint16_t *)pos_char, sizeof(pos_char));
             SCI_writeCharArray(mySCI0_BASE, (uint16_t *)("\r\n"), sizeof("\r\n"));
               if(EQEP_getDirection(EQEP1_BASE)>0)
               {
                   SCI_writeCharArray(mySCI0_BASE, (uint16_t *)("Positive"), sizeof("Positive"));
                   SCI_writeCharArray(mySCI0_BASE, (uint16_t *)("\r\n"), sizeof("\r\n"));
               }
               else
               {
                   SCI_writeCharArray(mySCI0_BASE, (uint16_t *)("Negative"), sizeof("Negative"));
                   SCI_writeCharArray(mySCI0_BASE, (uint16_t *)("\r\n"), sizeof("\r\n"));
               }

 }

void ConfigADC(uint32_t ADC_BASE)
{
    EALLOW;

    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_4_0);

#if(EX_ADC_RESOLUTION == 12)
    {
        ADC_setMode(ADC_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
    }
#elif(EX_ADC_RESOLUTION == 16)
    {
      ADC_setMode(ADCA_BASE, ADC_RESOLUTION_16BIT, ADC_MODE_DIFFERENTIAL);
    }
#endif
    ADC_setInterruptPulseMode(ADC_BASE, ADC_PULSE_END_OF_CONV);
    ADC_enableConverter(ADC_BASE);
    DEVICE_DELAY_US(1000);
    EDIS;
}




void initADC_SOC(void)
{
#if(EX_ADC_RESOLUTION == 12)
    {
        ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN0, 15);
        ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN2, 15);
        ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER4, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN4, 15);



    }
#elif(EX_ADC_RESOLUTION == 16)
    {
        ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN0,64);
    }
#endif
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER2);
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER4);


    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);

    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);


}


//
// End of File
//
