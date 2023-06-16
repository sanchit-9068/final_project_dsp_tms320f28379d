/*
 * cpu_timer.c
 *
 *  Created on: 13-Jun-2023
 *      Author: Sanchit Awasthi
 */

#include "cpu_timer.h"
#include "sci_init.h"



void
 initCPUTimers(void)
 {
     //
     // Initialize timer period to maximum
     //
     CPUTimer_setPeriod(CPUTIMER0_BASE, 0xFFFFFFFF);


     //
     // Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
     //
     CPUTimer_setPreScaler(CPUTIMER0_BASE, 0);

     //
     // Make sure timer is stopped
     //
     CPUTimer_stopTimer(CPUTIMER0_BASE);


     //
     // Reload all counter register with period value
     //
     CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);


     //
     // Reset interrupt counter
     //
//     cpuTimer0IntCount = 0;

 }


 void
 configCPUTimer(uint32_t cpuTimer, float freq, float period)
 {
     uint32_t temp;

     //
     // Initialize timer period:
     //
     temp = (uint32_t)(freq / 1000000 * period);
     CPUTimer_setPeriod(cpuTimer, temp);

     //
     // Set pre-scale counter to divide by 1 (SYSCLKOUT):
     //
     CPUTimer_setPreScaler(cpuTimer, 0);

     //
     // Initializes timer control register. The timer is stopped, reloaded,
     // free run disabled, and interrupt enabled.
     // Additionally, the free and soft bits are set
     //
     CPUTimer_stopTimer(cpuTimer);
     CPUTimer_reloadTimerCounter(cpuTimer);
     CPUTimer_setEmulationMode(cpuTimer,
                               CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
     CPUTimer_enableInterrupt(cpuTimer);

     //
     // Resets interrupt counters for the three cpuTimers
     //
//     if (cpuTimer == CPUTIMER0_BASE)
//     {
//         cpuTimer0IntCount = 0;
//     }

 }


