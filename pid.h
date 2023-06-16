/*
 * pid.h
 *
 *  Created on: 13-Jun-2023
 *      Author: Sanchit Awasthi
 */

#ifndef PID_H_
#define PID_H_

typedef struct
{
    float Kp;
    float Ki;
    float Kd;

    float tau;

    float limMin;
    float limMax;

    float limMinInt;
    float limMaxInt;
    float T;
    float integrator;
    float prevError;
    float differentiator;
    float prevMeasurement;

    float output;

} PIDController;

void PIDController_Init(PIDController *pid);
float PIDController_Update(PIDController *pid,float setpoint,float measurement);

#endif /* PID_H_ */
