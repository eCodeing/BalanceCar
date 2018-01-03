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
//	unsigned char  UND[4];
	unsigned char Encoder_L[2];
	unsigned char Encoder_R[2];
	
};
typedef  struct _FRAME  TX_Data;
struct _TX_ATTITUDE
{
	float angle;
	float gyro;
	float turn;
	float accelz;
};
typedef  struct _TX_ATTITUDE TX_Attitude;
struct _TX_ATTITUE_RAW
{
	float accel_x;
	float accel_y;
	float accel_z;
	float gyro_x;
	float gyro_y;
	float gyro_z;
};
typedef  TX_Data  RX_Data;
void Float2Char(u8 *buf,float num);
void Int2Char(u8 *buf,int num);
void Int2Char2(u8 *buf,uint16_t num);
void RX_Frame(void);
void TX_Frame(void);
void  TX_PackageFrame(void);
void Nrf_Check_Event(void);
#endif
