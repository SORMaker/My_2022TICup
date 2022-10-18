/*
 * pid.c
 *
 *  Created on: 2022��10��14��
 *      Author: Lenovo
 */

#include "headfile.h"

int16 num[8];


//�ٶȻ�
int16 PID_speed(int16 setspeed, int16 dat, float *PID)
{
    static int16 error1,error0=0;
    static int16 sum=0;
    int16 nowspeed;
    float m;
    int16 output;
    uint8 flag;

//    nowspeed = num[0];
//    nowspeed = (int16)((num[0] + num[1])/2);
    nowspeed = dat;

    error1 = setspeed - nowspeed;

    if(error1 >= integral_partitioning_limit || error1 <= -integral_partitioning_limit)
    {
        flag = 0;
        sum = 0;
    }
    else flag = 1;
//    flag = 1;

    sum += flag*error1;

    if(sum >= integral_limit) sum = integral_limit;
    if(sum <= -integral_limit) sum = -integral_limit;

    m = sum*PID[KI];

    if(error1 >= 600) error1 = 600;
    if(error1 <= -600) error1 = -600;

    output = (int16)(PID[KP]*error1 + m + PID[KD]*(error1 - error0));

    error0 = error1;

    if(output >= output_limit) output = output_limit;
    if(output <= -output_limit) output = -output_limit;

    return output;
}

int16 my_abs(int16 value)
{
    return value > 0 ? value : -value;
}

//��������ֵ
uint32 Read_speed(void)
{
    uint32 dis;

    num[0] = -gpt12_get(GPT12_T6);//��
    num[1] = gpt12_get(GPT12_T2);//��

    dis = (uint32)(0.5*my_abs(num[0])+0.5*my_abs(num[1]));

    gpt12_clear(GPT12_T2);
    gpt12_clear(GPT12_T6);

    return dis;
}

//����ת
void Direct(int16 left,int16 right)
{
    if(left <= 0)
    {
        gpio_set(P00_9, 0);
        pwm_duty(ATOM2_CH7_P20_7,-left);
    }
    else
    {
        gpio_set(P00_9, 1);
        pwm_duty(ATOM2_CH7_P20_7,left);
    }

    if(right <= 0)
    {
        gpio_set(P10_1, 1);
        pwm_duty(ATOM2_CH6_P20_6,-right);
    }
    else
    {
        gpio_set(P10_1, 0);
        pwm_duty(ATOM2_CH6_P20_6,right);
    }
}


//���� ���
int16 PID_steer(int16 error_now, float *PID)//[-752,752]
{
    int16 error_last = 0;
    int16 outdata; //[-200,200]
    int16 middata = 775;
//    float kp = 0.40;
//    float kd = 0.18;//!!!!�޸ĺ��޸���

    outdata = (int16)(PID[KP]*error_now + PID[KD]*(error_last-error_now));
    outdata = middata + outdata;

    if(outdata >= 850) outdata = 850;//��750
    if(outdata <= 700) outdata = 700;//��615
//    num[3] = outdata;

    error_last = error_now;

    return outdata;//[700,850]
}
