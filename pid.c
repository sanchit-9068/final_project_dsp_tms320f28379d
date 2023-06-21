/*
 * pid.c
 *
 *  Created on: 13-Jun-2023
 *      Author: Sanchit Awasthi
 */

#include "PID.h"

// Initialising the variables of the struct rest of them
// have to assigned in the calling function.

void PIDController_Init(PIDController *pid)
{
    pid->integrator=0.0f;
    pid->prevError=0.0f;
    pid->differentiator=0.0f;
    pid->prevMeasurement=0.0f;
    pid->output=0.0f;
}


//Update function

float PIDController_Update(PIDController *pid, float setpoint, float measurement)
{
    //error caclulation
    float error=setpoint-measurement;

    // proportional term calculation
    float proportional = (pid->Kp)*error;


    //integral term calculation
    (pid->integrator)+= 0.5f*(pid->Ki)*(pid->T)*(error+(pid->prevError));


    // anti windup limit for integral term limits
    if((pid->integrator)>(pid->limMaxInt))
    {
        pid->integrator=pid->limMaxInt;
    }
    else if ((pid->integrator)<(pid->limMinInt))
    {
        pid->integrator=pid->limMinInt;
    }


    // differentiator term calculation
//    pid->differentiator=-(2.0f * pid->Kd * (measurement - pid->prevMeasurement) /* Note: derivative on measurement, therefore minus sign in front of equation! */
//            + (2.0f * pid->tau - pid->T) * pid->differentiator)
//            / (2.0f * pid->tau + pid->T);

    // final output= proportional + integrator + derivative;
    pid->output= proportional+(pid->integrator);//+(pid->differentiator);


    // pid final output limits.
    if(pid->output > pid->limMax)
        pid->output=pid->limMax;
    else if (pid->output < pid->limMin)
        pid->output=pid->limMin;

    // new prev error= error
    // new prev measurement = measurement
    pid->prevError=error;
    pid->prevMeasurement=measurement;

    return pid->output;


}



