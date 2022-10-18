/*
 * camera.h
 *
 *  Created on: 2022年10月13日
 *      Author: Lenovo
 */

#ifndef CODE_CAMERA_H_
#define CODE_CAMERA_H_

#define Black 0
#define White 255
#define IMG_H MT9V03X_H
#define IMG_W MT9V03X_W

//extern uint8 threshold;

extern uint8 pixels[128][188];
extern uint8 gauss[128][188];//缓存数组

extern uint16 side_line[188];
extern uint16 right_line[128];
extern uint16 mid_line[128];

extern uint8 di_start;
extern uint8 di_end;
extern uint8 di_two;
extern uint8 di_three;
extern uint8 di_flag;


extern uint8 back_flag;

void gaussian(uint8 org_old[MT9V03X_H][MT9V03X_W], uint8 org[MT9V03X_H][MT9V03X_W]);

void Binarization(void);
void Search_Line(void);
void Find_Stop_1(void);
void Find_Stop_2(void);

int16 sum_error(int set_line);


#endif /* CODE_CAMERA_H_ */
