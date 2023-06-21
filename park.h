/*
 * park.h
 *
 *  Created on: 13-Jun-2023
 *      Author: Sanchit Awasthi
 */

#ifndef PARK_H_
#define PARK_H_

#include "driverlib.h"
#include "device.h"
#include "math.h"

float32_t park_id(float32_t ialpha,float32_t ibeta, float32_t theta);
float32_t park_iq(float32_t ialpha,float32_t ibeta, float32_t theta);




#endif /* PARK_H_ */
