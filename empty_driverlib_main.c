/*
 * Master source file
 *
 *  Created on: 13-Jun-2023
 *      Author: Sanchit Awasthi
 */
//
// Included Files
//
#include <encoder.h>
#include "driverlib.h"
#include "device.h"
#include "sci_init.h"
#include "cpu_timer.h"
//
// Main
//

__interrupt void cpuTimer0ISR(void);
uint16_t cpuTimer0IntCount;

int32_t prev_pos=0;

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
        //
        // Initialize the PIE vector table with pointers to the shell Interrupt
        // Service Routines (ISR).
        //
        Interrupt_initVectorTable();

        Interrupt_register(INT_TIMER0, &cpuTimer0ISR);
        initCPUTimers();
        configCPUTimer(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ, 1000);
        CPUTimer_enableInterrupt(CPUTIMER0_BASE);
        Interrupt_enable(INT_TIMER0);
        CPUTimer_startTimer(CPUTIMER0_BASE);

        EINT;
        ERTM;

        while (1)
        {

        }

        return 0;
}


__interrupt void
 cpuTimer0ISR(void)
 {
     cpuTimer0IntCount++;
     float speedRPM = calculateSpeed();
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



     //
     // Acknowledge this interrupt to receive more interrupts from group 1
     //
     Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
 }



//
// End of File
//
