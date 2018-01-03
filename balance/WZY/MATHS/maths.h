//***************************************************************************************
//烈火微型四轴飞行器源码版权归烈火团队所有，未经烈火团队同意，请勿随意在网上传播本源码。
//与本软件相关书籍<<四轴飞行器DIY-基于STM32微控制器>>，由北航出版社正式出版，内容对本套包
//含的所有软件以及硬件相关都做了详细的讲解，有兴趣的网友可以从各大书店购买。
//与本软件配套的硬件：http://fire-dragon.taobao.com
//如果有网友做了各种有意义的改进，请随时与我们保持联系。
//QQ：16053729    烈火QQ群：234879071
//***************************************************************************************
#ifndef _MATHS_H_
#define _MATHS_H_
#include "global.h"
//#include "struct_all.h"

/******************************************************************************
							宏定义
*******************************************************************************/ 
#define Pi	3.1415927f
#define Radian_to_Angle	   57.2957795f
#define RawData_to_Angle	0.0610351f	//以下参数对应2000度每秒
#define RawData_to_Radian	0.0010653f

/******************************************************************************
							全局函数声明
*******************************************************************************/ 
//void Get_Radian(struct _gyro *Gyro_in,struct _SI_float *Gyro_out);	
uint8_t Get_Checksum(uint8_t mydata[]);
float invSqrt(float x);

#endif

