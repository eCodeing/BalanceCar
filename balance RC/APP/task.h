#ifndef  __TASK_H
#define __TASK_H
#include  "global.h"



void  BSP_Init(void);
void  Task_OLED_Refresh(void);
void  Task_CalculateBatVal(void);
void  Task_1ms(void);
void  Task_2ms(void);
void  Task_4ms(void);
void  Task_5ms(void);
void Task_500HZ(void);
void Task_100HZ(void);
void Task_25HZ(void);
void Task_4HZ(void);
//void  Task_GetEulerAngle(ANGLE *eulerangle);
#endif

