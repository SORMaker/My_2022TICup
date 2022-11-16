/*
 * pid.h
 *
 *  Created on: 2022年10月14日
 *      Author: Lenovo
 */

#ifndef CODE_PID_H_
#define CODE_PID_H_

#define KP 0
#define KI 1
#define KD 2
#define integral_partitioning_limit 0//设定速度-10
#define integral_limit 8000
#define output_limit 8000

int16 PID_speed(int16 setspeed, int16 dat, float *PID);
uint32 Read_speed(void);
void Direct(int16 left,int16 right);
int16 PID_steer(int16 error, float *PID);
int16 my_abs(int16 value);

#endif /* CODE_PID_H_ */
