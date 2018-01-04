#ifndef  __UART3_H
#define __UART3_H
#include "sys.h"
#include "global.h"
/*uart3用于驱动蓝牙透传模块
*
*
*/

void UART3_init(u32 bound);
void UART3_SendData(char* buf);
#endif
