#ifndef   __ENCODER_H
#define  __ENCODER_H
#include "global.h"
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM4(void);
int16_t Read_Encoder(u8 TIMX) ;


#endif
