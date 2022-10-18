/*
 * motor.c
 *
 *  Created on: Oct 14, 2022
 *      Author: sorrymaker
 */
#include "headfile.h"

#define BLANK   0

#define REVERSE 1
#define BACK    2
#define STRIGHT 3
#define TURN    4

#define PARALLEL1    5
#define PARALLEL2    6
#define PARALLEL3    7
#define PARALLEL4    8

//#define speed_back_in   1500
//#define speed_back_out  1800
#define speed_stop   0

extern int speed_back_in;
extern int speed_back_out;

extern int task_flag;
extern int park_flag;
extern uint8 camera_flag;
extern uint16 di_count_5;
extern int flag_1s;
extern int16 num[8];
extern float Speed_PID_parameter[3];
void motor_init()
{
    //舵机
    gtm_pwm_init(ATOM1_CH1_P33_9, 50, 775);//中值775 向右——700 向左——850

    //电机
    gtm_pwm_init(ATOM2_CH6_P20_6, 10000, 0);//内侧PWM
    gtm_pwm_init(ATOM2_CH7_P20_7, 10000, 0);//外侧PWM
    gpio_init(P00_9, GPO, 1, PUSHPULL);//左轮DIR
    gpio_init(P10_1, GPO, 0, PUSHPULL);//右轮DIR
}

extern uint8 di_two;
extern uint8 stop_flag;

void Reverse_parking(uint8 *flag , uint32 *distance)
{
//    di_two = 1;
//    systick_delay_ms(STM1, 1000);
//    int16 left,right;
    if (*flag == REVERSE) {
//        systick_delay_ms(STM1, 1000);
        pwm_duty(ATOM1_CH1_P33_9,700);
        gpio_set(P00_9, 0);
        gpio_set(P10_1, 1);
        pwm_duty(ATOM2_CH6_P20_6,speed_back_in);
        pwm_duty(ATOM2_CH7_P20_7,speed_back_out);
       if (*distance>3000)
       {
           pwm_duty(ATOM1_CH1_P33_9,775);
//           left = PID_speed(0, num[0], Speed_PID_parameter);
//           right = PID_speed(0, num[1], Speed_PID_parameter);
//           Direct(left,right);
//           pwm_duty(ATOM2_CH6_P20_6,speed_stop);
//           pwm_duty(ATOM2_CH7_P20_7,speed_stop);
            *distance = 0;
            (*flag) = BACK;
       }
    }
}

extern uint8 di_flag_5;

void Back_car(uint8 *flag , uint32 *distance)
{
    int16 left,right;
    if (*flag == BACK) {
        pwm_duty(ATOM1_CH1_P33_9,775);
        pwm_duty(ATOM2_CH6_P20_6,speed_back_in);
        pwm_duty(ATOM2_CH7_P20_7,speed_back_in);
           if (*distance>550)
           {
               pwm_duty(ATOM1_CH1_P33_9,775);
               stop_flag = 1;

//               left = PID_speed(0, num[0], Speed_PID_parameter);
//               right = PID_speed(0, num[1], Speed_PID_parameter);
//               Direct(left,right);
//               pwm_duty(ATOM2_CH6_P20_6,speed_stop);
//               pwm_duty(ATOM2_CH7_P20_7,speed_stop);
                *distance = 0;
//                gpio_init(P02_7, GPO, 1, PUSHPULL);
                di_flag = 1;
//                systick_delay_ms(STM1, 100000);
//                gpio_init(P02_7, GPO, 0, PUSHPULL);
                (*flag) = STRIGHT;
           }
    }
}

void Go_stright(uint8 *flag , uint32 *distance)
{
    int16 left,right;
    if (*flag == STRIGHT)
    {
        pwm_duty(ATOM1_CH1_P33_9,775);
//        systick_delay_ms(STM1, 300);
        gpio_set(P00_9, 1);
        gpio_set(P10_1, 0);
        pwm_duty(ATOM2_CH6_P20_6,speed_back_in);
        pwm_duty(ATOM2_CH7_P20_7,speed_back_in);
           if (*distance>1000)//1100?????????????????????????????????????????????????????
           {
               pwm_duty(ATOM1_CH1_P33_9,700);
//               left = PID_speed(0, num[0], Speed_PID_parameter);
//               right = PID_speed(0, num[1], Speed_PID_parameter);
//               Direct(left,right);
//               pwm_duty(ATOM2_CH6_P20_6,speed_stop);
//               pwm_duty(ATOM2_CH7_P20_7,speed_stop);
                *distance = 0;
                (*flag) = TURN;
           }
    }
}

void Turn(uint8 *flag , uint32 *distance)
{
    int16 left,right;
    if (*flag == TURN) {
        pwm_duty(ATOM1_CH1_P33_9,700);
        pwm_duty(ATOM2_CH6_P20_6,speed_back_in);
        pwm_duty(ATOM2_CH7_P20_7,speed_back_out);
//        if (*distance > 3500 && *distance < 3700)
//        {
//            pwm_duty(ATOM1_CH1_P33_9,700);
////            left = PID_speed(0, num[0], Speed_PID_parameter);
////            right = PID_speed(0, num[1], Speed_PID_parameter);
////            Direct(left,right);
////            pwm_duty(ATOM2_CH6_P20_6,speed_stop);
////            pwm_duty(ATOM2_CH7_P20_7,speed_stop);
////            di_flag = 1;
//        }
//        if(*distance >= 4000 && *distance <=5000)
//        {
//            pwm_duty(ATOM1_CH1_P33_9,850);
//            di_flag = 1;
//        }
//            pwm_duty(ATOM1_CH1_P33_9,775);
        if(*distance > 3700)
        {
            pwm_duty(ATOM1_CH1_P33_9,800);
            *distance=0;
            *flag = BLANK;
            if(task_flag == 1)
            {
                *flag = 9;
            }
//            di_flag = 1;
            park_flag = 1;
            camera_flag = 1;
            di_count_5 = 0;
            flag_1s = 0;
        }//2400-3000
    }
}


//void Parallel_strigt(int *flag , uint32 *distance)
//{
//    if(*flag == PARALLEL0)
//    {
//        pwm_duty(ATOM1_CH1_P33_9,775);
//        pwm_duty(ATOM2_CH6_P20_6,1000);
//        pwm_duty(ATOM2_CH7_P20_7,1000);
//        if(*distance > 500)
//        {
//            pwm_duty(ATOM2_CH6_P20_6,0);
//            pwm_duty(ATOM2_CH7_P20_7,0);
//            *distance = 0;
//            *flag = PARALLEL1;
//        }
//    }
//}

void Parallel_park(uint8 *flag , uint32 *distance)
{
    int16 left,right;
    if (*flag == PARALLEL1) {
        pwm_duty(ATOM1_CH1_P33_9,700);
        gpio_set(P00_9, 0);
        gpio_set(P10_1, 1);
        pwm_duty(ATOM2_CH6_P20_6,speed_back_in);
        pwm_duty(ATOM2_CH7_P20_7,speed_back_out);
        if (*distance>1770) {
//            left = PID_speed(0, num[0], Speed_PID_parameter);
//            right = PID_speed(0, num[1], Speed_PID_parameter);
//            Direct(left,right);
//            pwm_duty(ATOM2_CH6_P20_6,speed_stop);
//            pwm_duty(ATOM2_CH7_P20_7,speed_stop);
            pwm_duty(ATOM1_CH1_P33_9,850);
            *distance = 0;
            *flag = PARALLEL2;
        }
    }
}

void Parallel_park_2(uint8 *flag , uint32 *distance)
{
    int16 left,right;
    if (*flag == PARALLEL2) {
//        di_flag = 1;
        pwm_duty(ATOM1_CH1_P33_9,850);
        gpio_set(P00_9, 0);
        gpio_set(P10_1, 1);
        pwm_duty(ATOM2_CH6_P20_6,speed_back_out);
        pwm_duty(ATOM2_CH7_P20_7,speed_back_in);
        if (*distance>1900) {
            stop_flag = 1;

//            left = PID_speed(0, num[0], Speed_PID_parameter);
//            right = PID_speed(0, num[1], Speed_PID_parameter);
//            Direct(left,right);
//            pwm_duty(ATOM2_CH6_P20_6,speed_stop);
//            pwm_duty(ATOM2_CH7_P20_7,speed_stop);
            pwm_duty(ATOM1_CH1_P33_9,775);
            di_flag = 1;
//            gpio_init(P02_7, GPO, 1, PUSHPULL);
//            systick_delay_ms(STM1, 1000);
//            gpio_init(P02_7, GPO, 0, PUSHPULL);
            *distance = 0;
            *flag = PARALLEL3;
        }
    }
}

void Parallel_park_3(uint8 *flag , uint32 *distance)
{
    if (*flag == PARALLEL3) {
        pwm_duty(ATOM1_CH1_P33_9,850);
        gpio_set(P00_9, 1);
        gpio_set(P10_1, 0);
            pwm_duty(ATOM2_CH6_P20_6,speed_back_out);
            pwm_duty(ATOM2_CH7_P20_7,speed_back_in);
            if (*distance>2000) {
                pwm_duty(ATOM1_CH1_P33_9,700);
                *distance = 0;
                *flag = PARALLEL4;
            }
        }
}

void Parallel_park_4(uint8 *flag , uint32 *distance)
{
    if(*flag == PARALLEL4)
    {
        pwm_duty(ATOM1_CH1_P33_9,700);
        gpio_set(P00_9, 1);
        gpio_set(P10_1, 0);
        pwm_duty(ATOM2_CH6_P20_6,speed_back_in);
        pwm_duty(ATOM2_CH7_P20_7,speed_back_out);
        if (*distance>2200)
        {
            pwm_duty(ATOM1_CH1_P33_9,775);
            *distance = 0;
            *flag = 9;
            camera_flag = 0;

        }
    }
}
