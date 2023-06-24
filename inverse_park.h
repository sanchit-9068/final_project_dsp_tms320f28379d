/*
 * inverse_park.h
 *
 *  Created on: 22-Jun-2023
 *      Author: Sanchit Awasthi
 */

#ifndef INVERSE_PARK_H_
#define INVERSE_PARK_H_

#include "driverlib.h"
#include "device.h"
#include "math.h"

float32_t inverse_park_alpha(float32_t id, float32_t iq, float32_t theta);
float32_t inverse_park_beta(float32_t id, float32_t iq, float32_t theta);


#endif /* INVERSE_PARK_H_ */
