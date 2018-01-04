#ifndef  __HMI_H
#define __HMI_H
#include "global.h"


void HMI_SendEOF(void);
void HMI_SendData(char* buf);
void HMI_DisplaySine(void);
void HMI_DisplayInfo(void);

#endif
