#ifndef  __CONTROL_H
#define __CONTROL_H
#include  "global.h"

#define PI 3.14159265
#define ZHONGZHI 0
#define  PWM_MAX  700
int balance(float Angle,float Gyro);
void Set_Pwm(int moto1,int moto2);
void Xianfu_Pwm(void);
#endif

