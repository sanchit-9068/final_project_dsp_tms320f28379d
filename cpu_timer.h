/*
 * cpu_timer.h
 *
 *  Created on: 13-Jun-2023
 *      Author: Sanchit Awasthi
 */

#ifndef CPU_TIMER_H_
#define CPU_TIMER_H_

#include "driverlib.h"
#include "device.h"

void initCPUTimers(void);
void configCPUTimer(uint32_t, float, float);



#endif /* CPU_TIMER_H_ */
