/*
 * encoder.h
 *
 *  Created on: 13-Jun-2023
 *      Author: Sanchit Awasthi
 */

#ifndef ENCODER_H_
#define ENCODER_H_


#include "driverlib.h"
#include "device.h"

#define ENCODER_RESOLUTION 4096   // Number of encoder counts per revolution
#define BASE_SPEED_RPM 1800       // Base speed of the motor in RPM



void setupEQEPModule(void);
void Pin_EQEP(void);



#endif /* ENCODER_H_ */
