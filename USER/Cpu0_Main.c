/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/


#include "headfile.h"
#pragma section all "cpu0_dsram"
//将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中

extern int16 num[8];
uint32 distance;

//工程导入到软件之后，应该选中工程然后点击refresh刷新一下之后再编译
//工程默认设置为关闭优化，可以自己右击工程选择properties->C/C++ Build->Setting
//然后在右侧的窗口中找到C/C++ Compiler->Optimization->Optimization level处设置优化等级
//一般默认新建立的工程都会默认开2级优化，因此大家也可以设置为2级优化

//对于TC系列默认是不支持中断嵌套的，希望支持中断嵌套需要在中断内使用enableInterrupts();来开启中断嵌套
//简单点说实际上进入中断后TC系列的硬件自动调用了disableInterrupts();来拒绝响应任何的中断，因此需要我们自己手动调用enableInterrupts();来开启中断的响应。
int core0_main(void)
{
	get_clk();//获取时钟频率  务必保留
	//用户在此处调用各种初始化函数等

	//蜂鸣器
    gpio_init(P02_7, GPO, 0, PUSHPULL);

    //车尾灯
    gpio_init(P20_11, GPO, 1, PUSHPULL);//左红灯
    gpio_init(P20_12, GPO, 1, PUSHPULL);//左黄灯
    gpio_init(P20_13, GPO, 1, PUSHPULL);//右红灯
    gpio_init(P20_14, GPO, 1, PUSHPULL);//右黄灯

    //电感
    adc_init(ADC_0, ADC0_CH2_A2);
    adc_init(ADC_0, ADC0_CH3_A3);

    //编码器
    gpt12_init(GPT12_T2, GPT12_T2INB_P33_7, GPT12_T2EUDB_P33_6);//右轮编码器 337为步进脉冲引脚
    gpt12_init(GPT12_T6, GPT12_T6INA_P20_3, GPT12_T6EUDA_P20_0);//左

    //PWM
    gtm_pwm_init(ATOM2_CH7_P20_7, 10000, 0);//ATOM 0模块的通道4 使用P02_4引脚输出PWM  PWM频率50HZ  占空比百分之0/GTM_ATOM0_PWM_DUTY_MAX*100  GTM_ATOM0_PWM_DUTY_MAX宏定义在zf_gtm_pwm.h
    gtm_pwm_init(ATOM2_CH6_P20_6, 10000, 0);//右轮pwm

    //DIR
    gpio_init(P00_9, GPO, 1, PUSHPULL);//左轮dir 1反0正
    gpio_init(P10_1, GPO, 0, PUSHPULL);//右轮dir

    //舵机
    gtm_pwm_init(ATOM1_CH1_P33_9, 50, 775);

    pit_interrupt_ms(CCU6_0, PIT_CH0, 10);
    pit_interrupt_ms(CCU6_0, PIT_CH1, 10);

    //等待所有核心初始化完毕
//	IfxCpu_emitEvent(&g_cpuSyncEvent);
//	IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);
	enableInterrupts();

	//注意 从V1.1.6版本之后  printf打印的信息从串口输出具体可以学习库例程6-Printf_Demo
    //注意 从V1.1.6版本之后  printf打印的信息从串口输出具体可以学习库例程6-Printf_Demo
    //注意 从V1.1.6版本之后  printf打印的信息从串口输出具体可以学习库例程6-Printf_Demo
	while (TRUE)
	{
		//用户在此处编写任务代码
//	    printf("%d\n",mid_line[10]);

	    //山外
        vcan_sendware(num, sizeof(num));
        systick_delay_ms(STM0, 20);
	}
}

#pragma section all restore


