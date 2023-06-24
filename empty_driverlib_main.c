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


__interrupt void cpuTimer0ISR(void);
__interrupt void cpuTimer1ISR(void);
uint16_t cpuTimer0IntCount;

int32_t prev_pos=0;

float32_t ialpha,ibeta,id,iq;
float32_t ia=1.00,ib=2.00;
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

float32_t iqref=0.0;
float32_t vq =0.0;
float32_t idref=0.0;
float32_t vd=0.0;


float32_t id_measured=0.0;
float32_t iq_measured=0.0;
float32_t Ta,Tb,Tc;

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
        configCPUTimer(CPUTIMER1_BASE, DEVICE_SYSCLK_FREQ, 20000);// interrupt every 20 millisecond
        CPUTimer_enableInterrupt(CPUTIMER0_BASE);
        CPUTimer_enableInterrupt(CPUTIMER1_BASE);
        Interrupt_enable(INT_TIMER0);
        Interrupt_enable(INT_TIMER1);
        CPUTimer_startTimer(CPUTIMER0_BASE);
        CPUTimer_startTimer(CPUTIMER1_BASE);

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



        pid_current_d.Kd = 0.0;
        pid_current_d.Ki = Kic;
        pid_current_d.Kp = Kpc;
        pid_current_q.limMax = MaxVq;
        pid_current_q.limMin = MinVq;
        pid_current_q.limMaxInt = MaxVq/2.00;
        pid_current_q.limMinInt = MinVq/2.00;
        PIDController_Init(&pid_current_d);




        pid_speed.Kd = 0.0;
        pid_speed.Ki = Kis;
        pid_speed.Kp = Kps;
        pid_speed.limMax = MaxCurrent;
        pid_speed.limMin = MinCurrent;
        pid_speed.limMaxInt = MaxCurrent/2.00;
        pid_speed.limMinInt = MinCurrent/2.00;
        PIDController_Init(&pid_speed);


        Ta = ta_pwm(0.5f, 0.5f);
        Tb = tb_pwm(0.5f, 0.5f);
        Tc = tc_pwm(0.5f, 0.5f);
//        seca = ta_pwm2(0.5, 0.5);
//   /     secb = tb_pwm2(0.5, 0.5);
//        secc = tc_pwm2(0.5, 0.5);
//        motor->PwmARegs->CMPA.bit.CMPA = (INV_PWM_HALF_TBPRD*motor->svgen.Ta)+INV_PWM_HALF_TBPRD;
        duty_a = Ta*INV_PWM_HALF_TBPRD + INV_PWM_HALF_TBPRD;
        duty_b = Tb*INV_PWM_HALF_TBPRD + INV_PWM_HALF_TBPRD;
        duty_c = Tc*INV_PWM_HALF_TBPRD + INV_PWM_HALF_TBPRD;

//        EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_A, (int16_t)duty_a);
//        EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_B, (int16_t)duty_a);
//        EPWM_setCounterCompareValue(myEPWM2_BASE, EPWM_COUNTER_COMPARE_A, (int16_t)duty_b);
//        EPWM_setCounterCompareValue(myEPWM2_BASE, EPWM_COUNTER_COMPARE_B, (int16_t)duty_b);
//        EPWM_setCounterCompareValue(myEPWM3_BASE, EPWM_COUNTER_COMPARE_A, (int16_t)duty_c);
//        EPWM_setCounterCompareValue(myEPWM3_BASE, EPWM_COUNTER_COMPARE_B, (int16_t)duty_c);
        duty_change(duty_a, duty_b, duty_c);


        while (1)
        {
            ialpha=clark_alpha(ia, ib);
            ibeta=clark_beta(ia, ib);
            id= park_id(ialpha, ibeta, M_PI/3.00);
            iq= park_iq(ialpha, ibeta, M_PI/3.00);

        }



        return 0;
}


__interrupt void
 cpuTimer0ISR(void)
 {


            PIDController_Update(&pid_speed, 500, speed_motor);
            iqref = pid_speed.output;
            PIDController_Update(&pid_current_q, iqref, iq_measured);
            vq = pid_current_q.output;
            PIDController_Update(&pid_current_d, 0.0, id_measured);
     //
     // Acknowledge this interrupt to receive more interrupts from group 1
     //
     Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
 }



__interrupt void
 cpuTimer1ISR(void)
 {

    cpuTimer0IntCount++;
        float speedRPM = calculateSpeed();
        speed_motor = speedRPM;
               uint32_t j=(int32_t)speedRPM;
               uint16_t m=(int16_t)j;
              char l[16]={};

               SCI_writeCharArray(mySCI0_BASE, (uint16_t *)("SPEED="), sizeof("SPEED="));
               intToStrPositive(m, l);
               SCI_writeCharArray(mySCI0_BASE, (uint16_t *)l, sizeof(l));
               SCI_writeCharArray(mySCI0_BASE, (uint16_t *)("\r\n"), sizeof("\r\n"));
    //            uint32_t qq=EQEP_getPosition(EQEP1_BASE);
    //            uint16_t qqq=(uint16_t)qq;
    //            char tt[16]={};
    //            SCI_writeCharArray(mySCI0_BASE, (uint16_t *)("Position="), sizeof("Position="));
    //            intToStrPositive(qqq, tt);
    //            SCI_writeCharArray(mySCI0_BASE, (uint16_t *)tt, sizeof(tt));
    //            SCI_writeCharArray(mySCI0_BASE, (uint16_t *)("\r\n"), sizeof("\r\n"));
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


//
// End of File
//
