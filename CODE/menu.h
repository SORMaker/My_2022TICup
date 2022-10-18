/*
 * menu.h
 *
 *  Created on: 2022年10月14日
 *      Author: Lenovo
 */
/*
 * menu.h
 *
 *  Created on: 2022年1月20日
 *      Author: Lenovo
 */

#ifndef CODE_MENU_H_
#define CODE_MENU_H_

//#include "key.h"

#define KeyDown 0
#define KeyUp 1

#define true 1
#define BUTTOM_UP_PIN     P13_2
#define BUTTOM_DOWN_PIN   P13_0
#define BUTTOM_LEFT_PIN   P13_1
#define BUTTOM_RIGHT_PIN  P13_3
#define BUTTOM_CENTER_PIN P11_2

#define BUTTOM_UP      P13_2
#define BUTTOM_DOWN    P13_0
#define BUTTOM_LEFT    P13_1
#define BUTTOM_RIGHT   P13_3
#define BUTTOM_CENTER  P11_2

#define Speed_P Speed_PID_parameter[0]
#define Speed_I Speed_PID_parameter[1]
#define Speed_D Speed_PID_parameter[2]


extern float Speed_PID_parameter[3];
extern float Direct_CS_PID_parameter[3];
extern float Direct_PID_parameter[3];
extern int16 LEFT_WHEEL_SPEED;
extern int16 RIGHT_WHEEL_SPEED;
extern float Speed;


void keyInit(void);
int key_check(PIN_enum pin);

void DisplayDou(uint16 x, uint16 y, float a);
void Clean_Key_Position(void);
void Show_Key_Position(uint8 x, uint8 y);
void Change_Key_Position(uint8 x, uint8 y) ;
void strcpy_rewrite(char *ch, const char *_ch);
void show_menu(uint8 *x, uint8 *y);
void show_menu2(uint8 *x, uint8 *y);
void Set_Parameters_list(float *Par);
void Set_Parameters_int(int *Par);
void Set_Treshold(void);
void Set_Back(void);
void Set_Debug_Param(void);
void Set_Debug_Param3(void);
void Change_Task(void);
void firstlist(void);



#endif /* CODE_MENU_H_ */
