#ifndef  __TASK_H
#define __TASK_H
#include  "global.h"

//void Task_StorePara(struct __Rc * rc);
//void Task_ReadPara(struct __Rc *rc);

void  BSP_Init(void);
void  Task_OLED_Refresh(void);
void  Task_CalculateBatVal(void);
void  Task_1ms(void);
void  Task_2ms(void);
void  Task_4ms(void);
void  Task_5ms(void);
void  Task_50ms(void);
void Task_10ms(void);
void Task_100ms(void);
void Task_100ms(void);
void  Task_500ms(void);
//void  Task_GetEulerAngle(ANGLE *eulerangle);
//void  Task_GetEulerAngle(ANGLE *eulerangle);
#endif

