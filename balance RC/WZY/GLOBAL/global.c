#include  "global.h"
//PWM相关
int pwm_moto;
//平衡车控制相关
//float Angle_Balance;
float Gyro_Balance;
float balance_kp=-75;
float balance_kd=-0.115;//kp -75 kp -0.1

float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
//串口3接收相关
uint8_t  Buffer_Uart3[BUFFER_UART3_LEN ];
uint16_t num_recv3;//因为uart3缓冲区不会超过512字节
//串口2接收相关
uint8_t  Buffer_Uart2[BUFFER_UART2_LEN ];
uint16_t num_recv2;//因为uart3缓冲区不会超过512字节
//定时器5中断次数
uint32_t Timer5_count = 0;
////电池电量,ADC采集相关
__IO uint16_t ADCConvertedValue;
float BatVal;
//编码器相关
int Encoder2Val;
int Encoder4Val ;
//NRF24L01 相关
u8 tmp_buf[33];
TX_Data  tx_frame;
RX_Data  rx_frame;
//标志位记录相关
u8 Flag_Stop=1,Flag_Show=0; //停止标志位和 显示标志位 默认停止 显示打开
u8 txdata_isok = 0;
u8 txdata_isfinished = 0;
u8 rxdata_isfinished = 1;
//任务计时变量
u8 Count_2ms,Count_10ms,Count_40ms,Count_250ms;
//记录解算出的欧拉角
ANGLE  balance_angle;
//
struct _Rc Rc;				//遥控通道  
