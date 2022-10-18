/*
 * camera.c
 *
 *  Created on: 2022年10月13日
 *      Author: Lenovo
 */

#include "headfile.h"
#include "math.h"

extern float threshold;//阈值
uint8 pixels[128][188];//缓存数组
uint8 gauss[128][188];//缓存数组
uint16 side_line[188];
uint16 min;

uint8 di_start = 0;
uint8 di_end = 0;
uint8 di_two = 0;
uint8 di_three = 0;
uint8 di_flag = 0;

uint8 T_times = 0;//T计时
uint8 T_count = 0;//T计数

uint8 camera_flag = 1;
uint8 T_flag = 0;
uint16 ku1;
uint8 ku_flag = 0;
uint8 back_flag = 0;
extern uint32 distance;
void Binarization(void)
{
    int i,j;
    uint8 Threshold;
//    threshold = 80;//(otsu()+5);//17//大津法值偏小  加一个固定值
    Threshold = (uint8)threshold;
    for(i = 0; i < 128; i++)
    {
        for(j = 0; j < 188; j++)
            pixels[i][j] = gauss[i][j] > Threshold ? 255 : 0;
    }
}

//==========高斯滤波=================
/*
 * 使用3*3 高斯算子进行滤波
 * {
 *    1 2 1
 *    2 4 2
 *    1 2 1
 * }
 * */
void gaussian(uint8 org_old[MT9V03X_H][MT9V03X_W], uint8 org[MT9V03X_H][MT9V03X_W])
{
    //最外一圈不做处理
    for (uint8 y=1; y< (MT9V03X_H - 1); y++)
    {
        for (uint8 x = 1; x < (MT9V03X_W - 1); x++)
        {
            //
            org[y][x] = ((int16)org_old[y-1][x-1] + (int16)org_old[y-1][x+1] + (int16)org_old[y+1][x-1] + (int16)org_old[y+1][x+1] +
                        2 * ((int16)org_old[y-1][x] + (int16)org_old[y][x-1] + (int16)org_old[y][x+1] + (int16)org_old[y+1][x]) +
                        4 * (int16)org_old[y][x])/16;
        }
    }
}

void Search_Line(void)
{
    uint16 i,j,k;
    min = 128;

    //偏置
    for(k = 0; k < 127; k++)
    {
        side_line[k] = 125;
    }

    //竖扫列 寻边线
    for(i = 1; i < 185; i++)//列
    {
        for(j = 120; j > 3; j--)//行
        {
            if((pixels[j-1][i] == Black)
                    && (pixels[j][i] == White)
                    && (pixels[j+1][i] == White))
            {
                side_line[i] = j;
                pixels[j][i] = 1;

                if(j < min && j > 10)
                {
                    min = j;
                }
                break;
            }
        }
    }

    //找到后显示至屏幕
//    for(j=0;j<IMG_W-5;j++)
//        pixels[min][j]=1;

//    pixels[20][20]=1;
//    pixels[20][20]=1;
//    pixels[20][20]=1;
//    pixels[20][20]=1;

//    //横扫行 寻库线
//    for(i = 30; i > 5; i -= 5)//行
//    {
//        for(j = 3; j < 185; j++)//列
//        {
//            if((pixels[i][j-1] == Black)
//                    || ((pixels[i][j-1] == White)
//                    && (pixels[i][j] == White)
//                    && (pixels[i][j+1] == Black)))
//            {
//                ku_flag = 1;
//                ku1 = j;
////                di_flag = 1;
//                break;
//            }
//        }
//
//        //break出 寻到库线且线处于后半部分
//        if(ku_flag && j > 130)
//        {
//            back_flag = 1;//到达倒车点 入库后清标
////            di_three = 1;
//            di_flag = 1;
//        }
//       //break出 寻到库线且线在前 继续寻找第二个库线
//        else if(ku_flag && j <= 150)
//        {
//            for(j = j+10; j < 185; j++)
//            {
//                if((pixels[i][j-1] == White)
//                        && (pixels[i][j] == White)
//                        && (pixels[i][j+1] == Black))
//                {
//                    ku_flag |= 2;
////                    di_flag = 1;
//                    break;
//                }
//            }
//
//            //扫到两库线 剩下的行不扫
//            if(ku_flag == 3)
//            {
////                ku_flag = 0;
//                break;
//            }
//        }
//        ku_flag = 0;
//
//    }
}

void Find_Stop_1(void)
{
    uint16 i,j;

    pixels[20][90]=2;
    pixels[50][90]=2;
    pixels[70][90]=2;
    pixels[50][70]=2;
    pixels[50][110]=2;

    //横扫行 寻库线
    for(i = 20; i > 5; i -= 5)//行
    {
        for(j = 3; j < 185; j++)//列
        {
            if((pixels[i][j-1] == Black)
                    || ((pixels[i][j-1] == White)
                    && (pixels[i][j] == White)
                    && (pixels[i][j+1] == Black)))
            {
                ku_flag = 1;
                ku1 = j;
//                di_flag = 1;
                break;
            }
        }

        //break出 寻到库线且线处于后半部分
        if(ku_flag && j > 150)
        {
            back_flag = 1;//到达倒车点 入库后清标
            camera_flag = 0;
//            di_three = 1;
//            di_flag = 1;
        }
       //break出 寻到库线且线在前 继续寻找第二个库线
        else if(ku_flag && j <= 130)
        {
            for(j = j+10; j < 185; j++)
            {
                if((pixels[i][j-1] == White)
                        && (pixels[i][j] == White)
                        && (pixels[i][j+1] == Black))
                {
                    ku_flag |= 2;
//                    di_flag = 1;
                    break;
                }
            }

            //扫到两库线 剩下的行不扫
            if(ku_flag == 3)
            {
//                ku_flag = 0;
                break;
            }
        }
        ku_flag = 0;

    }
}

extern int16 num[8];
uint16 black_count;

void Find_Stop_2(void)
{
    uint16 i;

//    for(j=70;j<110;j++)
//    {
//        pixels[50][j]=1;
//        pixels[70][j]=1;
//    }
//    for(j=50;j<70;j++)
//    {
//        pixels[j][70]=1;
//        pixels[j][110]=1;
//    }


    //还未识别到T 或 经过T后
    if(T_flag == 0)
    {
//        black_count = 0;
//        for(j = 70; j < 110; j++)
//        {
//            pixels[50][j]=1;
//            pixels[70][j]=1;
//        }
//        for(j = 50; j < 70; j++)
//        {
//            pixels[j][70]=1;
//            pixels[j][110]=1;
//        }
//
//        for(i = 50; i < 70; i++)
//        {
//            for(j = 70; j < 110; j++)
//            {
//                if(pixels[i][j] == 0)
//                {
//                    black_count++;
//                }
//            }
//        }
//
//        if(black_count > 500)
//        {
//            T_flag = 1;
//            T_count++;
//            di_two = 1;
//        }

//        if(side_line[90] > 15)
//        {
//            i = side_line[90] - 15;
//        }
//        else
//        {
//            i = 35;
//        }
//
//        for(j=0;j<180;j++)
//        {
//            pixels[i][j]=2;
//        }

        for(i = side_line[90] ; i > 8; i--)
        {
            if((pixels[i][90] == White)
                    && (pixels[i][92] == White)
                    && (pixels[i][94] == White))
            {
                num[5] = i;
//                black_count++;
                break;
            }
        }
//        if(white_count < side_line[90]/3)
        //无break 全黑
        if(i < side_line[90] - 20)
        {
            T_flag = 1;
            T_count++;
//            di_two = 1;
        }

    }
    //识别到T
    else if(T_flag)
    {
        //开始计帧
        T_times++;
        if(T_times > 10)//30//计帧清标 开始下一次识别
        {
            T_times = 0;
            T_flag = 0;
        }
    }

    //识别到三次T
    if(T_count >= 3)
    {
        T_count = 0;
        back_flag = 5;
        distance = 0;
        camera_flag = 0;
//        di_flag = 1;
    }



}

int16 sum_error(int set_line)
{
    int i,j;
    float array[60];
    double error=0;

    //权重 20 100行
    for(j = 0; j < 20; j++)//近
    {
        array[j] = 0.30;
    }
    for(j = 20; j < 40; j++)
    {
        array[j] = 0.40;
    }
    for(j = 40; j < 60; j++)
    {
        array[j] = 0.15;
    }
    for(j = 60; j < 80; j++)
    {
        array[j] = 0.15;
    }
    for(j = 80; j < 100; j++)//远
    {
        array[j] = 0.10;
    }

    //计算目标偏差
    for(i = 40, j = 0; i < 140; i++, j++)
    {
        //[0,128] 大于0 离得近 左转
        error += (side_line[i] - set_line) * 0.2;
//        error += (side_line[i] - 70) * array[j];
    }


    return (int16)error;//[-400,400]或更大
}
