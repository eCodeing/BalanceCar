#ifndef   __ENCODER_H
#define  __ENCODER_H
#include "global.h"
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM4(void);
int Read_Encoder(u8 TIMX);


#endif
