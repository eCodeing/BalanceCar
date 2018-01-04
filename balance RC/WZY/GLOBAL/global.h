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
#include "spi1.h"
#include "exti.h"
#include "ioiic.h"
#include "uart3.h"
#include "uart2.h"
#include "stdlib.h"
#include "string.h"
#include "timer.h"
#include "protocol.h"
#include "encoder.h"
#include "spi.h"
#include "task.h"
#include "hmi.h"
#include "uart_fifo.h"
#include "stmflash.h"
#include  "SPI_NRF24L01.h"
#include "inv_mpu_dmp_motion_driver.h" 
#define  BUFFER_UART3_LEN    512
#define  BUFFER_UART2_LEN    512
/* ң������ */
struct _Rc
{
	uint16_t THROTTLE;
	uint16_t YAW;
	uint16_t PITCH;
	uint16_t ROLL;
	
	uint16_t AUX1;
	uint16_t AUX2;
	uint16_t AUX3;
	uint16_t AUX4;
};
extern struct _Rc Rc;

struct  __ANGLE
{
	float Yaw;//�����
	float Roll;//�����
	float Pitch;//������
};
typedef struct  __ANGLE  ANGLE;
//����3�������
extern  uint8_t  Buffer_Uart3[ ];
extern uint16_t num_recv3;

//����2�������
extern  uint8_t  Buffer_Uart2[ ];
extern uint16_t num_recv2;
//NRF24L01 ���
extern u8 tmp_buf[33];
extern TX_Data  tx_frame;
extern RX_Data  rx_frame;
//���������
extern int Encoder2Val;
extern int Encoder4Val ;
//PWM���
extern int pwm_moto;
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
extern float Gyro_Balance;
extern float balance_kp;
extern float balance_kd;
//��ʱ��5�жϴ���
extern uint32_t Timer5_count;
//
extern u8 Count_2ms,Count_10ms,Count_40ms,Count_250ms;

//��¼�������ŷ����
extern ANGLE  balance_angle;
extern float q0,q1,q2,q3;

extern uint8_t processbar;


#endif
