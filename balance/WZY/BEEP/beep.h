#ifndef  __BEEP_H
#define  __BEEP_H
#include "sys.h"
//#define  BEEP(x)  x ? GPIO_SetBits(GPIOF,GPIO_Pin_8):GPIO_ResetBits(GPIOF,GPIO_Pin_8)
#define   BEEP    PFout(8)
void BEEP_Init(void);
#endif
