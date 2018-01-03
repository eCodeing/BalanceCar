#include  "global.h"
//PWM相关
int pwm_moto_l,pwm_moto_r;
int g_iCarSpeedSet = 0;
//平衡车控制相关
float Angle_Balance,Gyro_Balance,Gyro_Turn; //平衡倾角 平衡陀螺仪 转向陀螺仪
float Acceleration_Z;                       //Z轴加速度计  
//float Gyro_Balance;
float balance_kp= -80.0f;
float balance_kd= -0.12f;//kp max -90 kd max  -0.13
float balance_ki = 0.0f;
/*
*以下几组参数比较可以
kp:        -50           -44      -55.78   -53.59   -58     -58     -58   -58
ki:        -2.81         -2.51   -2.7       -1.89	  -2.61  -2.55  -2.5     -2.45
*/
float velocity_kp = -40.0f;//-10.0f
float velocity_ki = -1.1f;//-0.28f
//float velocity_kd = 0.0f;

float turn_kp = 0.0f;

float Target_Velocity = 90;
float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;

u8 Flag_Qian,Flag_Hou,Flag_Left,Flag_Right,Flag_sudu=2; //蓝牙遥控相关的变量
//串口3接收相关
uint8_t  Buffer_Uart3[BUFFER_UART3_LEN ];
uint16_t num_recv;//因为uart3缓冲区不会超过512字节
//定时器5中断次数
uint32_t Timer5_count = 0;
////电池电量,ADC采集相关
__IO uint16_t ADCConvertedValue;
float BatVal;
//编码器相关
int16_t Encoder2Val;
int16_t Encoder4Val ;
//NRF24L01 相关
u8 tmp_buf[33];
TX_Data  tx_frame;
//RX_Data  rx_frame;
TX_Attitude tx_attitude;
//标志位记录相关
u8 Flag_Stop=1,Flag_Show=0; //停止标志位和 显示标志位 默认停止 显示打开
//任务计时变量
u16 Count_1ms,Count_2ms,Count_4ms,Count_5ms,Count_50ms,Count_10ms,Count_100ms,Count_200ms,Count_500ms;
//记录解算出的欧拉角
ANGLE  balance_angle;
//
//struct _Rc Rc;				//遥控通道  
struct __Rc Rc;

