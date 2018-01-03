#ifndef  __GLOBAL_H
#define __GLOBAL_H
#include  "sys.h"
#include  "frame.h"
#include   "led.h"
#include   "math.h"
#include   "encoder.h"
#include   "control.h"
#include   "oled.h"
#include   "usart.h"
#include   "frame.h"
#include   "tb6612.h"
#include  "batval.h"
#include  "24l01.h"
#include  "mpu6050.h"
#include "inv_mpu.h"
#include "delay.h"
#include "beep.h"  
#include "key.h"
#include "filter.h"
#include "spi1.h"
#include "exti.h"
#include "ioiic.h"
#include "uart3.h"
#include "stdlib.h"
#include "string.h"
#include "timer.h"
#include "protocol.h"
#include "encoder.h"
#include "spi.h"
#include "task.h"
#include "maths.h"
#include "stmflash.h"
#include  "SPI_NRF24L01.h"
#include "inv_mpu_dmp_motion_driver.h" 
#define  BUFFER_UART3_LEN    512
struct __Rc
{
	float kp;
	float ki;
	float kd;
	char Dire_Front;
	char Dire_Back;
	char Dire_Left;
	char Dire_Right;
	uint16_t accel;
	char UND[14];
};
extern struct __Rc Rc;
struct  __ANGLE
{
	float Yaw;//�����
	float Roll;//�����
	float Pitch;//������
};
typedef struct  __ANGLE  ANGLE;
//����3�������
extern  uint8_t  Buffer_Uart3[ ];
extern uint16_t num_recv;
//NRF24L01 ���
extern u8 tmp_buf[33];
extern TX_Data  tx_frame;
extern TX_Attitude tx_attitude;
//���������
extern int16_t  Encoder2Val;
extern int16_t Encoder4Val ;
//PWM���
extern int pwm_moto_l,pwm_moto_r;
//��ص���,ADC�ɼ����
extern float BatVal;
extern __IO uint16_t ADCConvertedValue;
//��־λ��¼���
extern u8 Flag_Stop,Flag_Show;                               //ֹͣ��־λ�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��
extern u8 txdata_isok ;
extern u8 txdata_isfinished;
extern u8 rxdata_isfinished;
//ƽ�⳵�������
//extern float Angle_Balance;
extern float Angle_Balance,Gyro_Balance,Gyro_Turn; 
extern float Acceleration_Z;
extern float balance_kp;
extern float balance_kd;
extern float balance_ki;
extern float velocity_kp;
extern float velocity_ki;
extern float velocity_kd;
extern float turn_kp;
extern float Target_Velocity;
extern int g_iCarSpeedSet;
//��ʱ��5�жϴ���
extern uint32_t Timer5_count;
//
extern u16 Count_1ms,Count_2ms,Count_4ms,Count_5ms,Count_50ms,Count_10ms,Count_100ms,Count_200ms,Count_500ms;
extern u8 Flag_Qian,Flag_Hou,Flag_Left,Flag_Right,Flag_sudu;
//��¼�������ŷ����
extern ANGLE  balance_angle;
extern float q0,q1,q2,q3;
#endif
