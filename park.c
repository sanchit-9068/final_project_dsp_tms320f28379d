/*
 * park.c
 *
 *  Created on: 13-Jun-2023
 *      Author: Sanchit Awasthi
 */

#include "driverlib.h"
#include "device.h"
#include "park.h"
#include "math.h"

float32_t park_id(float32_t ialpha, float32_t ibeta, float32_t theta)
{
    float32_t cos_theta=cos(theta);
    float32_t sin_theta=sin(theta);

    float32_t result = ((ialpha*cos_theta) + (ibeta*sin_theta));
    return result;
}


float32_t park_iq(float32_t ialpha, float32_t ibeta, float32_t theta)
{
    float32_t cos_theta=cos(theta);
    float32_t sin_theta=sin(theta);

    float32_t result = ((ibeta*cos_theta) - (ialpha*sin_theta));
    return result;
}
