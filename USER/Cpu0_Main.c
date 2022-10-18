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
#pragma section all "cpu0_dsram"
//���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��

extern int16 num[8];
uint32 distance;

//���̵��뵽����֮��Ӧ��ѡ�й���Ȼ����refreshˢ��һ��֮���ٱ���
//����Ĭ������Ϊ�ر��Ż��������Լ��һ�����ѡ��properties->C/C++ Build->Setting
//Ȼ�����Ҳ�Ĵ������ҵ�C/C++ Compiler->Optimization->Optimization level�������Ż��ȼ�
//һ��Ĭ���½����Ĺ��̶���Ĭ�Ͽ�2���Ż�����˴��Ҳ��������Ϊ2���Ż�

//����TCϵ��Ĭ���ǲ�֧���ж�Ƕ�׵ģ�ϣ��֧���ж�Ƕ����Ҫ���ж���ʹ��enableInterrupts();�������ж�Ƕ��
//�򵥵�˵ʵ���Ͻ����жϺ�TCϵ�е�Ӳ���Զ�������disableInterrupts();���ܾ���Ӧ�κε��жϣ������Ҫ�����Լ��ֶ�����enableInterrupts();�������жϵ���Ӧ��
int core0_main(void)
{
	get_clk();//��ȡʱ��Ƶ��  ��ر���
	//�û��ڴ˴����ø��ֳ�ʼ��������

	//������
    gpio_init(P02_7, GPO, 0, PUSHPULL);

    //��β��
    gpio_init(P20_11, GPO, 1, PUSHPULL);//����
    gpio_init(P20_12, GPO, 1, PUSHPULL);//��Ƶ�
    gpio_init(P20_13, GPO, 1, PUSHPULL);//�Һ��
    gpio_init(P20_14, GPO, 1, PUSHPULL);//�һƵ�

    //���
    adc_init(ADC_0, ADC0_CH2_A2);
    adc_init(ADC_0, ADC0_CH3_A3);

    //������
    gpt12_init(GPT12_T2, GPT12_T2INB_P33_7, GPT12_T2EUDB_P33_6);//���ֱ����� 337Ϊ������������
    gpt12_init(GPT12_T6, GPT12_T6INA_P20_3, GPT12_T6EUDA_P20_0);//��

    //PWM
    gtm_pwm_init(ATOM2_CH7_P20_7, 10000, 0);//ATOM 0ģ���ͨ��4 ʹ��P02_4�������PWM  PWMƵ��50HZ  ռ�ձȰٷ�֮0/GTM_ATOM0_PWM_DUTY_MAX*100  GTM_ATOM0_PWM_DUTY_MAX�궨����zf_gtm_pwm.h
    gtm_pwm_init(ATOM2_CH6_P20_6, 10000, 0);//����pwm

    //DIR
    gpio_init(P00_9, GPO, 1, PUSHPULL);//����dir 1��0��
    gpio_init(P10_1, GPO, 0, PUSHPULL);//����dir

    //���
    gtm_pwm_init(ATOM1_CH1_P33_9, 50, 775);

    pit_interrupt_ms(CCU6_0, PIT_CH0, 10);
    pit_interrupt_ms(CCU6_0, PIT_CH1, 10);

    //�ȴ����к��ĳ�ʼ�����
//	IfxCpu_emitEvent(&g_cpuSyncEvent);
//	IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);
	enableInterrupts();

	//ע�� ��V1.1.6�汾֮��  printf��ӡ����Ϣ�Ӵ�������������ѧϰ������6-Printf_Demo
    //ע�� ��V1.1.6�汾֮��  printf��ӡ����Ϣ�Ӵ�������������ѧϰ������6-Printf_Demo
    //ע�� ��V1.1.6�汾֮��  printf��ӡ����Ϣ�Ӵ�������������ѧϰ������6-Printf_Demo
	while (TRUE)
	{
		//�û��ڴ˴���д�������
//	    printf("%d\n",mid_line[10]);

	    //ɽ��
        vcan_sendware(num, sizeof(num));
        systick_delay_ms(STM0, 20);
	}
}

#pragma section all restore

