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
#pragma section all "cpu1_dsram"
//将本语句与#pragma section all restore语句之间的全局变量都放在CPU1的RAM中

//定义拨码开关引脚
#define SW1     P10_3

extern int16 num[8];
extern uint8 camera_flag;
extern uint32 distance;
extern uint16 di_count_5;

extern uint8 task1_wid;
extern uint8 ku_flag;
extern uint16 ku1;

extern uint8 task2_wid;
extern uint8 T_count;
extern uint8 T_times;

extern uint16 black_count;

int16 camera_error;
int16 steer_duty;

extern uint8 di_flag;
extern int park_flag;
extern int task_flag;
extern uint8 stop_flag;

void core1_main(void)
{
	disableInterrupts();
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    //用户在此处调用各种初始化函数等
    lcd_init(); //初始化IPS屏幕
    lcd_showstr(0, 0, "SEEKFREE MT9V03x");
    lcd_showstr(0, 1, "Initializing...");
    //如果屏幕没有任何显示，请检查屏幕接线
    firstlist();
    mt9v03x_init(); //初始化摄像头
    //拨码开关状态变量
    uint8 sw1_status;
    enableInterrupts();
    while (TRUE)
    {

        if(camera_flag)
        {
            //用户在此处编写任务代码
            if(mt9v03x_finish_flag)
            {
                gaussian(mt9v03x_image, gauss);

                Binarization();
                Search_Line();
                if(park_flag == 0)
                {
                    Find_Stop_1();
                    //方向偏差 控制舵机
                    camera_error = sum_error(task1_wid);
                }
                else if(park_flag == 1)
                {
                    Find_Stop_2();
                    camera_error = sum_error(task2_wid);
                }
                //方向偏差 控制舵机
                steer_duty = PID_steer(camera_error,Direct_PID_parameter);
                pwm_duty(ATOM1_CH1_P33_9,steer_duty);
                lcd_showint16(0,7,task_flag);
                lcd_showint16(50,7,park_flag);

                sw1_status= gpio_get(SW1);

                if(sw1_status)
                {
                    lcd_displayimage032(pixels[0], MT9V03X_W, MT9V03X_H);

                }
                mt9v03x_finish_flag = 0;//在图像使用完毕后  务必清除标志位，否则不会开始采集下一幅图像
                //注意：一定要在图像使用完毕后在清除此标志位
            }
        }
    }
}



#pragma section all restore
