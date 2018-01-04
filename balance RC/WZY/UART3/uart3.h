#ifndef  __UART2_H
#define __UART2_H
#include "sys.h"
#include "global.h"
/*uart3用于驱动蓝牙透传模块
*
*
*/
void UART2_init(u32 bound);
void UART2_SendData(char* buf);
#endif
