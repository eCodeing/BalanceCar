#ifndef  __CONTROL_H
#define __CONTROL_H
#include  "global.h"

#define PI 3.14159265
#define ZHONGZHI  (0)
#define  PWM_MAX  800
int balance(float Angle,float Gyro);
int velocity(int encoder_left,int encoder_right);
int turn(int encoder_left,int encoder_right,float gyro);
void Set_Pwm(int moto1,int moto2);
void Xianfu_Pwm(void);
void Get_Angle(u8 way);
#endif

