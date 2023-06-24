/*
 * clark.c
 *
 *  Created on: 13-Jun-2023
 *      Author: Sanchit Awasthi
 */


#include "driverlib.h"
#include "device.h"
#include "IQmathLib.h"
#include "clark.h"


float32_t onebysqrt3=0.5773502692;

float32_t clark_alpha(float32_t ia, float32_t ib)
{
    return ia;
}

float32_t clark_beta(float32_t ia,float32_t ib)
{

    return ((ib+2*ia)*onebysqrt3);
}
