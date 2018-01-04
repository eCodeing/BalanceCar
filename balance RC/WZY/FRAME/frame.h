#ifndef  __FRAME_H
#define __FRAME_H
#include  "sys.h"
struct  _FRAME
{
	unsigned char Angle_Balance[4];//
	unsigned char Gyro_Balance[4];//
	unsigned char BatVal[4];
	unsigned char PWM_Moto[4];
	unsigned char  KP[4];
	unsigned char  KI[4];
	unsigned char  KD[4];
	unsigned char  UND[4];
};
typedef  struct _FRAME  TX_Data;
typedef  TX_Data  RX_Data;
void Float2Char(u8 *buf,float num);
void Int2Char(u8 *buf,int num);
void RX_Frame(void);
void TX_Frame(void);
void  TX_PackageFrame(void);
void Nrf_Check_Event(void);
#endif
