
 
/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		isr
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/


#include "isr_config.h"
#include "isr.h"

#define SW2     P11_1

extern int16 num[8];
uint8 di_count=0;
uint16 di_count_5=0;
uint8 di_flag_5 = 0;
uint8 di_two_count = 0;
uint8 di_three_count = 0;
uint8 go_flag = 0;
extern uint32 distance;
int16 middle_value[2];

void Di_Control(void)
{
    if(di_start == 1)
    {
        gpio_set(P02_7,1);
        if(di_end == 1)
        {
            di_start = 0;
            di_end = 0;
            gpio_set(P02_7,0);
        }
    }
    else if(di_two == 1)
    {
        di_two_count++;
        if(di_two_count >= 0 && di_two_count < 20)
        {
            gpio_set(P02_7,1);
        }
        else if(di_two_count >= 20 && di_two_count < 30)
        {
            gpio_set(P02_7,0);
        }
        else if(di_two_count >= 30 && di_two_count < 50)
        {
            gpio_set(P02_7,1);
        }
        else if(di_two_count >= 50)
        {
            gpio_set(P02_7,0);
            di_two = 0;
            di_two_count = 0;
        }
    }
    else if(di_three == 1)
    {
        di_two_count++;
        if(di_two_count >= 0 && di_two_count < 10)
        {
            gpio_set(P02_7,1);
        }
        else if(di_two_count >= 10 && di_two_count < 15)
        {
            gpio_set(P02_7,0);
        }
        else if(di_two_count >= 15 && di_two_count < 25)
        {
            gpio_set(P02_7,1);
        }
        else if(di_two_count >= 25 && di_two_count < 30)
        {
            gpio_set(P02_7,0);
        }
        else if(di_two_count >= 30 && di_two_count < 40)
        {
            gpio_set(P02_7,1);
        }
        else if(di_two_count >= 40)
        {
            gpio_set(P02_7,0);
            di_three = 0;
            di_three_count = 0;
        }
    }
    else if(di_flag == 1)
    {
        gpio_set(P02_7,1);
        di_count = di_count + 1;
        if(di_count > 40)
        {
            di_flag = 0;
            di_count = 0;
            gpio_set(P02_7,0);
        }
    }
    else if(di_flag_5 == 1)
    {
//        gpio_set(P02_7,1);
//        di_count_5 = di_count_5 + 1;
//        if(di_count > 100)
//        {
//            di_flag_5 = 0;
//            di_count = 0;
////            gpio_set(P02_7,0);
//        }
    }
}



extern uint8 camera_flag;
 int flag_1s = 1;
 uint8 flag_mode = 1;
 uint8 stop_flag = 0;
//PIT中断函数  示例
IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
	enableInterrupts();//开启中断嵌套
	PIT_CLEAR_FLAG(CCU6_0, PIT_CH0);

    uint8 sw2_status;
	int16 left,right;


	distance += Read_speed();

    if(stop_flag)
    {
        left = PID_speed(0, num[0], Speed_PID_parameter);
        right = PID_speed(0, num[1], Speed_PID_parameter);
        Direct(left,right);
    }

    if(back_flag == 1 ||back_flag == 2 || back_flag == 3 || back_flag == 4)
	{
//	    count_times++;

        if(flag_1s < 145)
        {
            left = PID_speed(0, num[0], Speed_PID_parameter);
            right = PID_speed(0, num[1], Speed_PID_parameter);
            Direct(left,right);
//            systick_delay_ms(STM1,25000);
            distance = 0;
            flag_1s++;
            gpio_set(P20_11, 1);
            gpio_set(P20_13, 1);
        }
        if(flag_1s == 145)
        {
            di_flag = 1;
            flag_1s++;
            gpio_set(P20_11, 0);
            gpio_set(P20_13, 0);
            gpio_set(P20_12, 1);
            gpio_set(P20_14, 1);
        }
//        systick_delay_ms(STM1,25000);
        if(flag_1s >= 145)
        {
//            di_flag = 1;
            Reverse_parking(&back_flag , &distance);
            Back_car(&back_flag, &distance);
            if(back_flag == 3)
            {
                di_count_5++;
                gpio_set(P20_12, 0);
                gpio_set(P20_14, 0);
            }
            if(di_count_5 > 500)
            {
                stop_flag = 0;
                Go_stright(&back_flag,&distance);
                Turn(&back_flag,&distance);
            }
        }
	}
	else if(back_flag == 5 || back_flag == 6 || back_flag == 7 || back_flag == 8)
	{
//	    Parallel_strigt(&back_flag , &distance);

	    if(distance < 1450 && flag_mode)
	    {
	        left = PID_speed(Speed, num[0], Speed_PID_parameter);
	        right = PID_speed(Speed, num[1], Speed_PID_parameter);
	        Direct(left,right);
	    }
	    else if(flag_1s < 145)
        {
	        flag_mode = 0;
            left = PID_speed(0, num[0], Speed_PID_parameter);
            right = PID_speed(0, num[1], Speed_PID_parameter);
            Direct(left,right);
//            systick_delay_ms(STM1,25000);
            distance = 0;
            flag_1s++;
            gpio_set(P20_11, 1);
            gpio_set(P20_13, 1);
        }
	    if(flag_1s == 145)
	    {
            gpio_set(P20_11, 0);
            gpio_set(P20_13, 0);
	        di_flag = 1;
	        flag_1s++;
            gpio_set(P20_12, 1);
            gpio_set(P20_14, 1);
	    }
////        systick_delay_ms(STM1,25000);
        if(flag_1s >= 145)
        {
            Parallel_park(&back_flag , &distance);
            Parallel_park_2(&back_flag, &distance);
            if(back_flag == 7)
            {
                gpio_set(P20_12, 0);
                gpio_set(P20_14, 0);
                di_count_5++;

            }
            if(di_count_5 > 500)
            {
                Parallel_park_3(&back_flag,&distance);
                Parallel_park_4(&back_flag,&distance);
            }

        }
//        flag = 0;
	}
	else if(back_flag == 9)
	{
	    left = PID_speed(0, num[0], Speed_PID_parameter);
        right = PID_speed(0, num[1], Speed_PID_parameter);
        Direct(left,right);
	}
	else if(back_flag != 9 || back_flag != 5 || back_flag != 1)
	{
	    left = PID_speed(Speed, num[0], Speed_PID_parameter);
        right = PID_speed(Speed, num[1], Speed_PID_parameter);
        Direct(left,right);
	}




    sw2_status= gpio_get(SW2);

//    if(sw2_status)
//    {
//        Di_Control();

//    }


}


IFX_INTERRUPT(cc60_pit_ch1_isr, 0, CCU6_0_CH1_ISR_PRIORITY)
{
	enableInterrupts();//开启中断嵌套
	PIT_CLEAR_FLAG(CCU6_0, PIT_CH1);
    Di_Control();

}

IFX_INTERRUPT(cc61_pit_ch0_isr, 0, CCU6_1_CH0_ISR_PRIORITY)
{
	enableInterrupts();//开启中断嵌套
	PIT_CLEAR_FLAG(CCU6_1, PIT_CH0);
//	distance += Read_speed();

}

IFX_INTERRUPT(cc61_pit_ch1_isr, 0, CCU6_1_CH1_ISR_PRIORITY)
{
	enableInterrupts();//开启中断嵌套
	PIT_CLEAR_FLAG(CCU6_1, PIT_CH1);

}




IFX_INTERRUPT(eru_ch0_ch4_isr, 0, ERU_CH0_CH4_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
	if(GET_GPIO_FLAG(ERU_CH0_REQ4_P10_7))//通道0中断
	{
		CLEAR_GPIO_FLAG(ERU_CH0_REQ4_P10_7);
	}

	if(GET_GPIO_FLAG(ERU_CH4_REQ13_P15_5))//通道4中断
	{
		CLEAR_GPIO_FLAG(ERU_CH4_REQ13_P15_5);
	}
}

IFX_INTERRUPT(eru_ch1_ch5_isr, 0, ERU_CH1_CH5_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
	if(GET_GPIO_FLAG(ERU_CH1_REQ5_P10_8))//通道1中断
	{
		CLEAR_GPIO_FLAG(ERU_CH1_REQ5_P10_8);
	}

	if(GET_GPIO_FLAG(ERU_CH5_REQ1_P15_8))//通道5中断
	{
		CLEAR_GPIO_FLAG(ERU_CH5_REQ1_P15_8);
	}
}

//由于摄像头pclk引脚默认占用了 2通道，用于触发DMA，因此这里不再定义中断函数
//IFX_INTERRUPT(eru_ch2_ch6_isr, 0, ERU_CH2_CH6_INT_PRIO)
//{
//	enableInterrupts();//开启中断嵌套
//	if(GET_GPIO_FLAG(ERU_CH2_REQ7_P00_4))//通道2中断
//	{
//		CLEAR_GPIO_FLAG(ERU_CH2_REQ7_P00_4);
//
//	}
//	if(GET_GPIO_FLAG(ERU_CH6_REQ9_P20_0))//通道6中断
//	{
//		CLEAR_GPIO_FLAG(ERU_CH6_REQ9_P20_0);
//
//	}
//}



IFX_INTERRUPT(eru_ch3_ch7_isr, 0, ERU_CH3_CH7_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
	if(GET_GPIO_FLAG(ERU_CH3_REQ6_P02_0))//通道3中断
	{
		CLEAR_GPIO_FLAG(ERU_CH3_REQ6_P02_0);
		if		(CAMERA_GRAYSCALE == camera_type)	mt9v03x_vsync();
		else if (CAMERA_BIN_UART  == camera_type)	ov7725_uart_vsync();
		else if	(CAMERA_BIN       == camera_type)	ov7725_vsync();

	}
	if(GET_GPIO_FLAG(ERU_CH7_REQ16_P15_1))//通道7中断
	{
		CLEAR_GPIO_FLAG(ERU_CH7_REQ16_P15_1);

	}
}



IFX_INTERRUPT(dma_ch5_isr, 0, ERU_DMA_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套

	if		(CAMERA_GRAYSCALE == camera_type)	mt9v03x_dma();
	else if (CAMERA_BIN_UART  == camera_type)	ov7725_uart_dma();
	else if	(CAMERA_BIN       == camera_type)	ov7725_dma();
}


//串口中断函数  示例
IFX_INTERRUPT(uart0_tx_isr, 0, UART0_TX_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrTransmit(&uart0_handle);
}
IFX_INTERRUPT(uart0_rx_isr, 0, UART0_RX_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrReceive(&uart0_handle);
}
IFX_INTERRUPT(uart0_er_isr, 0, UART0_ER_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrError(&uart0_handle);
}

//串口1默认连接到摄像头配置串口
IFX_INTERRUPT(uart1_tx_isr, 0, UART1_TX_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrTransmit(&uart1_handle);
}
IFX_INTERRUPT(uart1_rx_isr, 0, UART1_RX_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrReceive(&uart1_handle);
    if		(CAMERA_GRAYSCALE == camera_type)	mt9v03x_uart_callback();
    else if (CAMERA_BIN_UART  == camera_type)	ov7725_uart_callback();
}
IFX_INTERRUPT(uart1_er_isr, 0, UART1_ER_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrError(&uart1_handle);
}


//串口2默认连接到无线转串口模块
IFX_INTERRUPT(uart2_tx_isr, 0, UART2_TX_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrTransmit(&uart2_handle);
}
IFX_INTERRUPT(uart2_rx_isr, 0, UART2_RX_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrReceive(&uart2_handle);
    switch(wireless_type)
    {
    	case WIRELESS_SI24R1:
    	{
    		wireless_uart_callback();
    	}break;

    	case WIRELESS_CH9141:
		{
		    bluetooth_ch9141_uart_callback();
		}break;
    	default:break;
    }

}
IFX_INTERRUPT(uart2_er_isr, 0, UART2_ER_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrError(&uart2_handle);
}



IFX_INTERRUPT(uart3_tx_isr, 0, UART3_TX_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrTransmit(&uart3_handle);
}
IFX_INTERRUPT(uart3_rx_isr, 0, UART3_RX_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrReceive(&uart3_handle);
}
IFX_INTERRUPT(uart3_er_isr, 0, UART3_ER_INT_PRIO)
{
	enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrError(&uart3_handle);
}
