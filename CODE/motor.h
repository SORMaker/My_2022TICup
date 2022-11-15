/*
 * motor.h
 *
 *  Created on: 2022Äê10ÔÂ15ÈÕ
 *      Author: sorrymaker
 */

#ifndef CODE_MOTOR_H_
#define CODE_MOTOR_H_

void motor_init();
void Reverse_parking(uint8 *flag , uint32 *distance);
void Back_car(uint8 *flag , uint32 *distance);
void Go_stright(uint8 *flag , uint32 *distance);
void Turn(uint8 *flag , uint32 *distance);
void Parallel_park(uint8 *flag , uint32 *distance);
void Parallel_park_2(uint8 *flag , uint32 *distance);
void Parallel_park_3(uint8 *flag , uint32 *distance);
void Parallel_park_4(uint8 *flag , uint32 *distance);


#endif /* CODE_MOTOR_H_ */
