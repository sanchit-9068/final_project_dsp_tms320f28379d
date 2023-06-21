/*
 * clark.h
 *
 *  Created on: 13-Jun-2023
 *      Author: Sanchit Awasthi
 */

#ifndef CLARK_H_
#define CLARK_H_

#include "driverlib.h"
#include "device.h"
#include "IQmathLib.h"


float32_t clark_alpha(float32_t ia,float32_t ib);
float32_t clark_beta(float32_t ia,float32_t ib);




#endif
