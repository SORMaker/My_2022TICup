/*
 * menu.c
 *
 *  Created on: 2022年10月14日
 *      Author: Lenovo
 */

/*
 * menu.c
 *
 *  Created on: 2022年1月20日
 *      Author: Lenovo
 */

#include "headfile.h"

#define delay(x) systick_delay_ms(STM0, x)

float Speed_PID_parameter[3] = {95, 20, 0};//速度环PID参数25.0, 8.0, 0//55.0, 0.5, 0 /P95
float Direct_CS_PID_parameter[3] = {2, 0, 5.0};//方向环差速PID参数未调
float Direct_PID_parameter[3] = {0.2 , 0, 0};//0.3//方向环PID参数//zuo0.40//you0.42//0.40, 0, 0.18

int16 LEFT_WHEEL_SPEED;//num[0]
int16 RIGHT_WHEEL_SPEED;//num[1]
float Speed = 35;
float threshold = 140;//task_1 200
int speed_back_in = 2350;//2300
int speed_back_out = 2470;//2490
int park_flag = 0;
int task_flag = 0;
int task1_wid = 30;
int task2_wid = 40;


void keyInit(void)
{
    gpio_init     (BUTTOM_UP    ,GPI,1,PULLUP); //设为输入方向,上拉
    gpio_init     (BUTTOM_DOWN  ,GPI,1,PULLUP); //设为输入方向,上拉
    gpio_init     (BUTTOM_LEFT  ,GPI,1,PULLUP); //设为输入方向,上拉
    gpio_init     (BUTTOM_RIGHT ,GPI,1,PULLUP); //!!!!!!!!!!!!!!!!!!!!右键0变1 设为输入方向,上拉
    gpio_init     (BUTTOM_CENTER,GPI,1,PULLUP); //设为输入方向,上拉
}


int key_check(PIN_enum pin)
{
    if (gpio_get(pin)==KeyDown)
    {
        delay(5);
        return gpio_get(pin);
    }
    return KeyUp;
}

void DisplayDou(uint16 x, uint16 y, float a)
{
    char str[20];
    sprintf(str, "000%.4f", a-(int)a);
    lcd_showstr(x, y, str);
    sprintf(str, "%04d", (int)a);
    lcd_showstr(x, y, str);
}


/*在一级或部分菜单中去除右边五向导航的位置标志*/
void Clean_Key_Position(void)
{
    uint8 i = 0;
    for (i = 0; i < 7; i++)
    {
        lcd_showstr(120, i, " ");
    }
}

/*在修改PID参数时显示五向导航光标*/
void Show_Key_Position(uint8 x, uint8 y)
{
    lcd_showstr(x, y, "^");
}

/*在修改PID参数时改变五向导航光标*/
void Change_Key_Position(uint8 x, uint8 y)
{
    uint16 a;
    for (a = 0; a < 100; a++)
    {
        lcd_showstr(a, 1, "  ");
    }
    Show_Key_Position(x, y);
}

/************************************************
  功能说明：uint8类型字符串的赋值函数
  参数说明：uint8形字符串首地址，常字符串首地址
  制作者：YB
  修改日期：17/12/01
  参考调用：strcpy_rewrite(str,"asd");
           可将字符串asd赋值给字符串变量str
*************************************************/
void strcpy_rewrite(char *ch, const char *_ch)
{
    while ((*ch++ = *_ch++) != '\0');
}

/*
 * 显示主菜单
 */
void show_menu(uint8 *x, uint8 *y)
{
    char str[22];
    *x = 0, *y = 0;
    lcd_clear(WHITE);
    lcd_showstr(*x, *y, "TRAVELER_LXZ");

    *x = 0;
    (*y)++;
    strcpy(str, "1.Threshold");
    lcd_showstr(*x, *y, str);

    *x = 0;
    (*y)++;
    strcpy_rewrite(str, "2.Back Speed");
    lcd_showstr(*x, *y, str);

    *x = 0;
    (*y)++;
    strcpy_rewrite(str, "3.Speed PID");
    lcd_showstr(*x, *y, str);

    *x = 0;
    (*y)++;
    strcpy_rewrite(str, "4.Direct PID");
    lcd_showstr(*x, *y, str);

    *x = 0;
    (*y)++;
    strcpy_rewrite(str, "5.Task");
    lcd_showstr(*x, *y, str);

    *x = 0;
    (*y)++;
    strcpy_rewrite(str, "6.Go!!!");
    lcd_showstr(*x, *y, str);
}

/*
 * 显示任务切换菜单
 */
void show_menu2(uint8 *x, uint8 *y)
{
    char str[22];
    *x = 0, *y = 0;
    lcd_clear(WHITE);
    lcd_showstr(*x, *y, "Task_Change");

    *x = 0;
    (*y)++;
    strcpy(str, "1.Task All");
    lcd_showstr(*x, *y, str);

    *x = 0;
    (*y)++;
    strcpy_rewrite(str, "2.Task 1");
    lcd_showstr(*x, *y, str);

    *x = 0;
    (*y)++;
    strcpy_rewrite(str, "3.Task 2");
    lcd_showstr(*x, *y, str);

    *x = 0;
    (*y)++;
    strcpy_rewrite(str, "4.Back");
    lcd_showstr(*x, *y, str);

    *x = 0;
    (*y)++;
    strcpy_rewrite(str, "task");
    lcd_showstr(*x, *y, str);
    DisplayDou(*(x + 65), *y, (float) task_flag);

    *x = 0;
    (*y)++;
    strcpy_rewrite(str, "park");
    lcd_showstr(*x, *y, str);
    DisplayDou(*(x + 65), *y, (float) park_flag);
}

//修改浮点型参数
void Set_Parameters_list(float *Par)
{
#define MAX_OPTION 9
#define END_X (MAX_OPTION-1)*6

    uint8 x = 0, y = 0;
    uint8 i = 0, j = 0;

    lcd_clear(WHITE);
    DisplayDou(x, y, *Par);
    x = 0;
    y++;
    Change_Key_Position(x, y);

    delay(500);

    while (j != 2)
    {
        delay(150);
        if (key_check(BUTTOM_UP) == KeyDown)
        {
            switch (x)
            {
                case 0 * 6:
                    (*Par) += 1000;
                    break;
                case 1 * 6:
                    (*Par) += 100;
                    break;
                case 2 * 6:
                    (*Par) += 10;
                    break;
                case 3 * 6:
                    (*Par) += 1;
                    break;
                case 4 * 6:
                    break;
                case 5 * 6:
                    (*Par) += 0.1;
                    break;
                case 6 * 6:
                    (*Par) += 0.01;
                    break;
                case 7 * 6:
                    (*Par) += 0.001;
                    break;
                case 8 * 6:
                    (*Par) += 0.0001;
                    break;
            }
            i = x;
            x = 0;
            y = 0;
            DisplayDou(x, y, *Par);
            x = i;
            y++;
            Change_Key_Position(x, y);
        }
        if (key_check(BUTTOM_DOWN) == KeyDown)
        {
            switch (x)
            {
                case 0 * 6:
                    (*Par) -= 1000;
                    break;
                case 1 * 6:
                    (*Par) -= 100;
                    break;
                case 2 * 6:
                    (*Par) -= 10;
                    break;
                case 3 * 6:
                    (*Par) -= 1;
                    break;
                case 4 * 6:
                    break;
                case 5 * 6:
                    (*Par) -= 0.1;
                    break;
                case 6 * 6:
                    (*Par) -= 0.01;
                    break;
                case 7 * 6:
                    (*Par) -= 0.001;
                    break;
                case 8 * 6:
                    (*Par) -= 0.0001;
                    break;
            }
            i = x;
            x = 0;
            y = 0;
            DisplayDou(x, y, *Par);
            x = i;
            y++;
            Change_Key_Position(x, y);
        }
        if (key_check(BUTTOM_LEFT) == KeyDown)
        {
            if (x == 0) x = END_X;
            else x -= 6;
            Change_Key_Position(x, y);
        }
        if (key_check(BUTTOM_RIGHT) == KeyDown)
        {
            if (x == END_X) x = 0;
            else x += 6;
            Change_Key_Position(x, y);
        }

        if (key_check(BUTTOM_CENTER) == KeyDown)
        {
            j = 2;
        }
    }
#undef END_X
#undef MAX_OPTION
}

//修改整形数据
void Set_Parameters_int(int *Par)
{
#define MAX_OPTION 7
#define END_X (MAX_OPTION-1)*6

    uint8 x = 0, y = 0;
    uint8 i = 0, j = 0;

    lcd_clear(WHITE);

    char str[20];
    sprintf(str, "%04d", *Par);
    lcd_showstr(0, 0, str);

    x = 0;
    y++;
    Change_Key_Position(x, y);

    delay(500);

    while (j != 2)
    {
        delay(150);
        if (key_check(BUTTOM_UP) == KeyDown)
        {
            switch (x) {
                case 0 * 6:
                    (*Par) += 1000;
                    break;
                case 1 * 6:
                    (*Par) += 100;
                    break;
                case 2 * 6:
                    (*Par) += 10;
                    break;
                case 3 * 6:
                    (*Par) += 1;
                    break;
            }
            i = x;
            x = 0;
            y = 0;
            sprintf(str, "%04d", *Par);
            lcd_showstr(0, 0, str);
            x = i;
            y++;
            Change_Key_Position(x, y);
        }
        if (key_check(BUTTOM_DOWN) == KeyDown)
        {
            switch (x) {
                case 0 * 6:
                    (*Par) -= 1000;
                    break;
                case 1 * 6:
                    (*Par) -= 100;
                    break;
                case 2 * 6:
                    (*Par) -= 10;
                    break;
                case 3 * 6:
                    (*Par) -= 1;
                    break;
            }
            i = x;
            x = 0;
            y = 0;
            sprintf(str, "%04d", *Par);
            lcd_showstr(0, 0, str);
            x = i;
            y++;
            Change_Key_Position(x, y);
        }
        if (key_check(BUTTOM_LEFT) == KeyDown)
        {
            if (x == 0) x = END_X;
            else x -= 6;
            Change_Key_Position(x, y);
        }
        if (key_check(BUTTOM_RIGHT) == KeyDown)
        {
            if (x == END_X) x = 0;
            else x += 6;
            Change_Key_Position(x, y);
        }

        if (key_check(BUTTOM_CENTER) == KeyDown)
        {
            j = 2;
        }
    }
#undef END_X
#undef MAX_OPTION
}

//菜单部分
//屏幕显示
void Set_Treshold(void)
{
    uint8 x = 0, y = 0;
    lcd_clear(WHITE);
    delay(100);

    while (true)
    {
        lcd_showstr(60,3,"EXCITING!");
        if(key_check(BUTTOM_CENTER) == KeyDown)
            break;
    }
    lcd_clear(WHITE);
    show_menu(&x, &y);
}

//调阈值
void Set_Threshold(void)
{
#define OPTION_NUM 2
#define OPTION_INDEX (OPTION_NUM-1)
#define START_ROW 1
    uint8 j = 0;
    lcd_clear(WHITE);
    char str[22];
    uint8 x = 0, y = 0;

    strcpy_rewrite(str, "Threshold");
    lcd_showstr(x, y, str);
    DisplayDou(x + 80, y, (float) threshold);

    x = 0;
    y++;
    strcpy_rewrite(str, "1.Set Threshold");
    lcd_showstr(x, y, str);

    x = 0;
    y++;
    strcpy_rewrite(str, "2.back");
    lcd_showstr(x, y, str);

    delay(500);

    while (1)
    {
        if (key_check(BUTTOM_UP) == KeyDown && j != 0)
            j--;
        else if (key_check(BUTTOM_UP) == KeyDown && j == 0)
            j += OPTION_INDEX;
        if (key_check(BUTTOM_DOWN) == KeyDown && j != OPTION_INDEX)
            j++;
        else if (key_check(BUTTOM_DOWN) == KeyDown && j == OPTION_INDEX)
            j -= OPTION_INDEX;

        Clean_Key_Position();
        strcpy_rewrite(str, "*");
        lcd_showstr(120, j + START_ROW, str);

        if (key_check(BUTTOM_CENTER) == KeyDown)
        {
            switch (j)
            {
                case 0:
                    Set_Parameters_list(&threshold);
                    break;
                case 1:
                    break;
            }
            show_menu(&x, &y);
            break;
        }

        delay(200);
    }
#undef OPTION_NUM
#undef OPTION_INDEX
#undef START_ROW
}

//倒车内外轮速度调节
void Set_Back(void)
{
#define OPTION_NUM 3
#define OPTION_INDEX (OPTION_NUM-1)
#define START_ROW 2
    uint8 j = 0;
    lcd_clear(WHITE);
    char str[22];
    uint8 x = 0, y = 0;

    strcpy_rewrite(str, "BS_IN");
    lcd_showstr(x, y, str);
    DisplayDou(x + 65, y, (float) speed_back_in);

    x = 0;
    y++;
    strcpy_rewrite(str, "BS_OUT");
    lcd_showstr(x, y, str);
    DisplayDou(x + 65, y, (float) speed_back_out);

    x = 0;
    y++;
    strcpy_rewrite(str, "1.Set BS_IN");
    lcd_showstr(x, y, str);

    x = 0;
    y++;
    strcpy_rewrite(str, "2.Set BS_OUT");
    lcd_showstr(x, y, str);

    x = 0;
    y++;
    strcpy_rewrite(str, "3.back");
    lcd_showstr(x, y, str);

    delay(500);

    while (1)
    {
        if (key_check(BUTTOM_UP) == KeyDown && j != 0)
            j--;
        else if (key_check(BUTTOM_UP) == KeyDown && j == 0)
            j += OPTION_INDEX;
        if (key_check(BUTTOM_DOWN) == KeyDown && j != OPTION_INDEX)
            j++;
        else if (key_check(BUTTOM_DOWN) == KeyDown && j == OPTION_INDEX)
            j -= OPTION_INDEX;

        Clean_Key_Position();
        strcpy_rewrite(str, "*");
        lcd_showstr(120, j + START_ROW, str);

        if (key_check(BUTTOM_CENTER) == KeyDown)
        {
            switch (j)
            {
                case 0:
                    Set_Parameters_int(&speed_back_in);
                    break;
                case 1:
                    Set_Parameters_int(&speed_back_out);
                    break;
                case 3:
                    break;
            }
            show_menu(&x, &y);
            break;
        }

        delay(200);
    }
#undef OPTION_NUM
#undef OPTION_INDEX
#undef START_ROW
}

//速度环PID调参
void Set_Debug_Param(void)
{
#define OPTION_NUM 4
#define OPTION_INDEX (OPTION_NUM-1)
#define START_ROW 3
    uint8 j = 0;
    lcd_clear(WHITE);
    char str[22];
    uint8 x = 0, y = 0;

    strcpy_rewrite(str, "Speed_P");
    lcd_showstr(x, y, str);
    DisplayDou(x + 65, y, (float) Speed_P);

    x = 0;
    y++;
    strcpy_rewrite(str, "Speed_I");
    lcd_showstr(x, y, str);
    DisplayDou(x + 65, y, (float) Speed_I);


    x = 0;
    y++;
    strcpy_rewrite(str, "Speed");
    lcd_showstr(x, y, str);
    DisplayDou(x + 65, y, (float) Speed);


    x = 0;
    y++;
    strcpy_rewrite(str, "1.Set Speed_P");
    lcd_showstr(x, y, str);

    x = 0;
    y++;
    strcpy_rewrite(str, "2.Set Speed_I");
    lcd_showstr(x, y, str);


    x = 0;
    y++;
    strcpy_rewrite(str, "3.Set Speed");
    lcd_showstr(x, y, str);


    x = 0;
    y++;
    strcpy_rewrite(str, "4.back");
    lcd_showstr(x, y, str);

    delay(500);

    while (1)
    {
        if (key_check(BUTTOM_UP) == KeyDown && j != 0)
            j--;
        else if (key_check(BUTTOM_UP) == KeyDown && j == 0)
            j += OPTION_INDEX;
        if (key_check(BUTTOM_DOWN) == KeyDown && j != OPTION_INDEX)
            j++;
        else if (key_check(BUTTOM_DOWN) == KeyDown && j == OPTION_INDEX)
            j -= OPTION_INDEX;

        Clean_Key_Position();
        strcpy_rewrite(str, "*");
        lcd_showstr(120, j + START_ROW, str);

        if (key_check(BUTTOM_CENTER) == KeyDown)
        {
            switch (j)
            {
                case 0:
                    Set_Parameters_list(&Speed_P);
                    break;
                case 1:
                    Set_Parameters_list(&Speed_I);
                    break;
                case 2:
                    Set_Parameters_list(&Speed);
                    break;
                case 3:
                    break;
            }
            show_menu(&x, &y);
            break;
        }

        delay(200);
    }
#undef OPTION_NUM
#undef OPTION_INDEX
#undef START_ROW
}

//方向环PID
void Set_Debug_Param3(void)
{
#define OPTION_NUM 4
#define OPTION_INDEX (OPTION_NUM-1)
#define START_ROW 3
    uint8 j = 0;
    lcd_clear(WHITE);
    char str[22];
    uint8 x = 0, y = 0;

    strcpy_rewrite(str, "Direct_P");
    lcd_showstr(x, y, str);
    DisplayDou(x + 65, y, Direct_PID_parameter[0]);

    x = 0;
    y++;
    strcpy_rewrite(str, "Direct_I");
    lcd_showstr(x, y, str);
    DisplayDou(x + 65, y, Direct_PID_parameter[1]);


    x = 0;
    y++;
    strcpy_rewrite(str, "Direct_D");
    lcd_showstr(x, y, str);
    DisplayDou(x + 65, y, Direct_PID_parameter[2]);


    x = 0;
    y++;
    strcpy_rewrite(str, "1.Set Direct_P");
    lcd_showstr(x, y, str);

    x = 0;
    y++;
    strcpy_rewrite(str, "2.Set Direct_I");
    lcd_showstr(x, y, str);


    x = 0;
    y++;
    strcpy_rewrite(str, "3.Set Direct_D");
    lcd_showstr(x, y, str);

    x = 0;
    y++;
    strcpy_rewrite(str, "4.Back");
    lcd_showstr(x, y, str);

    delay(500);

    while (1)
    {
        if (key_check(BUTTOM_UP) == KeyDown && j != 0)
            j--;
        else if (key_check(BUTTOM_UP) == KeyDown && j == 0)
            j += OPTION_INDEX;
        if (key_check(BUTTOM_DOWN) == KeyDown && j != OPTION_INDEX)
            j++;
        else if (key_check(BUTTOM_DOWN) == KeyDown && j == OPTION_INDEX)
            j -= OPTION_INDEX;

        Clean_Key_Position();
        strcpy_rewrite(str, "*");
        lcd_showstr(120, j + START_ROW, str);

        if (key_check(BUTTOM_CENTER) == KeyDown)
        {
            switch (j)
            {
                case 0:
                    Set_Parameters_list(&Direct_PID_parameter[0]);
                    break;
                case 1:
                    Set_Parameters_list(&Direct_PID_parameter[1]);
                    break;
                case 2:
                    Set_Parameters_list(&Direct_PID_parameter[2]);
                    break;
                case 3:
                    break;
            }
            show_menu(&x, &y);
            break;
        }

        delay(200);
    }
#undef OPTION_NUM
#undef OPTION_INDEX
#undef START_ROW
}

//任务切换
void Change_Task(void)
{
#define OPTION_NUM 4 //选项数
#define START_ROW 1 //选项开始的行
#define OPTION_INDEX (OPTION_NUM-1)
    uint8 i = 0, j = 0;
    uint8 x = 0, y = 0;
    show_menu2(&x, &y);

    delay(500);

    while (i != 1)
    {
        delay(200);
        if (key_check(BUTTOM_UP) == KeyDown && j != 0)
        {
            j--;
        } else if (key_check(BUTTOM_UP) == KeyDown && j == 0)
        {
            j += OPTION_INDEX;
        }
        if (key_check(BUTTOM_DOWN) == KeyDown && j != OPTION_INDEX)
        {
            j++;
        } else if (key_check(BUTTOM_DOWN) == KeyDown && j == OPTION_INDEX)
        {
            j = 0;
        }
        Clean_Key_Position();
        lcd_showstr(120, j + START_ROW, "*");

        if (key_check(BUTTOM_CENTER) == KeyDown)
        {
            switch (j)
            {
                case 0:
                    task_flag = 0;
                    park_flag = 0;
                    break;
                case 1:
                    task_flag = 1;
                    park_flag = 0;
                    break;
                case 2:
                    task_flag = 2;
                    park_flag = 1;
                    break;
                case 3:
                    lcd_clear(WHITE);
                    i = 1;
            }
            show_menu(&x, &y);
            break;
        }
    }
#undef  OPTION_NUM
#undef OPTION_INDEX
#undef START_ROW
}


/*一级菜单*/
void firstlist(void)
{
#define OPTION_NUM 6 //选项数
#define START_ROW 1 //选项开始的行
#define OPTION_INDEX (OPTION_NUM-1)
    uint8 i = 0, j = 0;
    uint8 x = 0, y = 0;
    show_menu(&x, &y);

    delay(500);

    while (i != 1)
    {
        delay(200);
        if (key_check(BUTTOM_UP) == KeyDown && j != 0)
        {
            j--;
        } else if (key_check(BUTTOM_UP) == KeyDown && j == 0)
        {
            j += OPTION_INDEX;
        }
        if (key_check(BUTTOM_DOWN) == KeyDown && j != OPTION_INDEX)
        {
            j++;
        } else if (key_check(BUTTOM_DOWN) == KeyDown && j == OPTION_INDEX)
        {
            j = 0;
        }
        Clean_Key_Position();
        lcd_showstr(120, j + START_ROW, "*");

        if (key_check(BUTTOM_CENTER) == KeyDown)
            switch (j)
            {
                case 0:
                    Set_Threshold();
                    break;
                case 1:
                    Set_Back();
                    break;
                case 2:
                    Set_Debug_Param();
                    break;
                case 3:
                    Set_Debug_Param3();
                    break;
                case 4:
                    Change_Task();
                    break;
                case 5:
                    lcd_clear(WHITE);
                    i = 1;
            }
    }
#undef  OPTION_NUM
#undef OPTION_INDEX
#undef START_ROW
}




