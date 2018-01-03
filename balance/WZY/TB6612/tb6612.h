#ifndef  __TB6612_H
#define __TB6612_H

#include "sys.h"

void  TB6612_Init(void);
void  Run_Forward(uint32_t PWM_L,uint32_t PWM_R);
void  Run_Backward(uint32_t PWM_L,uint32_t PWM_R);
void  Moto_LeftRun_Backward(uint32_t PWM_L);
void  Moto_LeftRun_Forward(uint32_t PWM_L);
void  Moto_RightRun_Backward(uint32_t PWM_R);
void  Moto_RightRun_Forward(uint32_t PWM_R);
#endif
