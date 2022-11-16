/*
 * pid.c
 *
 *  Created on: 2022年10月14日
 *      Author: Lenovo
 */

#include "headfile.h"

int16 num[8];


//速度环
int16 PID_speed(int16 setspeed, int16 dat, float *PID)
{
    static int16 error1,error0=0;
    static int16 sum=0;
    int16 nowspeed;
    float m;
    int16 output;
    nowspeed = dat;//当前速度

    error1 = setspeed - nowspeed;//误差

    //积分分离
    if(error1 >= integral_partitioning_limit || error1 <= -integral_partitioning_limit)
    {
        sum = 0;//清零积分项，防止出现下次开始积分时，还保留上次的积分项
    }
    else
    {
        sum += error1;
    }

    //积分限幅
    if(sum >= integral_limit) sum = integral_limit;
    if(sum <= -integral_limit) sum = -integral_limit;

    m = sum*PID[KI];//单独拿出来没啥特别含义，单纯为了看i效果

    output = (int16)(PID[KP]*error1 + m + PID[KD]*(error1 - error0));

    error0 = error1;//误差更新

    //输出限幅
    if(output >= output_limit) output = output_limit;
    if(output <= -output_limit) output = -output_limit;

    return output;
}

//绝对值函数 用于编码器积路程
int16 my_abs(int16 value)
{
    return value > 0 ? value : -value;
}

//编码器积路程
uint32 Read_speed(void)
{
    uint32 dis;

    num[0] = -gpt12_get(GPT12_T6);//左
    num[1] = gpt12_get(GPT12_T2);//右

    dis = (uint32)(0.5*my_abs(num[0])+0.5*my_abs(num[1]));

    gpt12_clear(GPT12_T2);
    gpt12_clear(GPT12_T6);

    return dis;
}

//正反转
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


//方向环 舵机
int16 PID_steer(int16 error_now, float *PID)//[-752,752]
{
    int16 error_last = 0;
    int16 outdata; //[-200,200]
    int16 middata = 775;
//    float kp = 0.40;
//    float kd = 0.18;//!!!!修改后无负号

    outdata = (int16)(PID[KP]*error_now + PID[KD]*(error_last-error_now));
    outdata = middata + outdata;//中值加pid数值

    if(outdata >= 850) outdata = 850;//左850
    if(outdata <= 700) outdata = 700;//右700

    error_last = error_now;

    return outdata;//[700,850]
}

