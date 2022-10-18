/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ����������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/

#include "headfile.h"
#pragma section all "cpu1_dsram"
//���������#pragma section all restore���֮���ȫ�ֱ���������CPU1��RAM��

//���岦�뿪������
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
    //�û��ڴ˴����ø��ֳ�ʼ��������


    lcd_init(); //��ʼ��IPS��Ļ
    lcd_showstr(0, 0, "SEEKFREE MT9V03x");
    lcd_showstr(0, 1, "Initializing...");
    //�����Ļû���κ���ʾ��������Ļ����
    firstlist();
    mt9v03x_init(); //��ʼ������ͷ
//    int i;
    //���뿪��״̬����
    uint8 sw1_status;

    //�ȴ����к��ĳ�ʼ�����
//	IfxCpu_emitEvent(&g_cpuSyncEvent);
//	IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);
    enableInterrupts();
    while (TRUE)
    {

        if(camera_flag)
        {
            //�û��ڴ˴���д�������
            if(mt9v03x_finish_flag)
            {
                gaussian(mt9v03x_image, gauss);

                Binarization();
                Search_Line();
                if(park_flag == 0)
                {
                    Find_Stop_1();
                    //����ƫ�� ���ƶ��
                    camera_error = sum_error(task1_wid);
                }
                else if(park_flag == 1)
                {
                    Find_Stop_2();
                    camera_error = sum_error(task2_wid);
                }
                //����ƫ�� ���ƶ��
//                camera_error = sum_error(task2_wid);
                steer_duty = PID_steer(camera_error,Direct_PID_parameter);
                pwm_duty(ATOM1_CH1_P33_9,steer_duty);

    //            printf("&&&&&&&&&&\n");
    //            //��ӡ����
    //            for(i = 0; i < 188; i++)
    //                printf("%d,\n",side_line[i]);
    //            printf("&&&&&&&&&&\n");

                //task_1
//                lcd_showuint16(50,7,ku1);
//                lcd_showint16(100,7,steer_duty);
//                lcd_showint16(100,6,camera_error);

                //task_2
//                lcd_showuint8(0,7,T_count);
//                lcd_showuint8(50,7,T_times);
//                lcd_showint16(100,7,num[5]);
//                lcd_showint16(100,6,black_count);

                lcd_showint16(0,7,task_flag);
                lcd_showint16(50,7,park_flag);

                sw1_status= gpio_get(SW1);

                if(sw1_status)
                {
                    lcd_displayimage032(pixels[0], MT9V03X_W, MT9V03X_H);

                }
                mt9v03x_finish_flag = 0;//��ͼ��ʹ����Ϻ�  ��������־λ�����򲻻Ὺʼ�ɼ���һ��ͼ��
                //ע�⣺һ��Ҫ��ͼ��ʹ����Ϻ�������˱�־λ
            }
        }
//        lcd_showint16(0,7,task_flag);
//        lcd_showint16/(50,7,park_flag);
//        lcd_showuint16(0,7,di_count_5);

    }
}



#pragma section all restore