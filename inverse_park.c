/*
 * inverse_park.c
 *
 *  Created on: 22-Jun-2023
 *      Author: Sanchit Awasthi
 */


#include "inverse_park.h"

float32_t inverse_park_alpha(float32_t id, float32_t iq, float32_t theta)
{
    float32_t cos_theta=cos(theta);
    float32_t sin_theta=sin(theta);

    float32_t result = (cos_theta*id)-(sin_theta*iq);
    return result;
}


float32_t inverse_park_beta(float32_t id, float32_t iq, float32_t theta)
{
    float32_t cos_theta=cos(theta);
    float32_t sin_theta=sin(theta);

    float32_t result = (sin_theta*id)+(iq*cos_theta);
    return result;
}

