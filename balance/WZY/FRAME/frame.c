#include  "frame.h"
#include "global.h"
//format:+/-xxx.xxx
void Float2Char(u8 *buf,float num)
{
	int16_t temp = 0;
	char num_isnative = 0;
	if(num>0.0f)
	{
		buf[0] = '+';
	    temp = (int16_t)num;
	} 
	else 
	{
		buf[0] = '-';
	    temp = (int16_t)(-num);
		num_isnative = 1;
	} 
	buf[1] = (temp>>8)&0xFF;
	buf[2] = (temp>>0)&0xFF;
	if(num_isnative)temp = (int16_t)(((-num) - temp)*100);
	else temp = (int16_t)((num - temp)*100);
	buf[3] = temp;
}
void Int2Char(u8 *buf,int num)
{
	int temp = 0;
	if(num>=0)
	{
		buf[0] = '+';
	    temp = num;
	} 
	else 
	{
		buf[0] = '-';
	    temp = (-num);
	} 
	buf[1] = ((temp>>8)&0xFF);
	buf[2] = ((temp>>0)&0xFF);
	buf[3] = 0xFF;
}
void Int2Char2(u8 *buf,uint16_t num)
{
	buf[0] = ((num>>8)&0xFF);
	buf[1] = ((num>>0)&0xFF);
}
void  TX_PackageFrame(void)
{
		Float2Char(tx_frame.Angle_Balance,Angle_Balance);
		Float2Char(tx_frame.Gyro_Balance,Gyro_Balance);
		Int2Char(tx_frame.PWM_Moto,pwm_moto_l);
	    Float2Char(tx_frame.BatVal,(float)BatVal*7.7f);
		Float2Char(tx_frame.KP,Rc.kp);
//		Float2Char(tx_frame.KP,velocity_kp);
		Float2Char(tx_frame.KI,Rc.ki);
		Float2Char(tx_frame.KD,Rc.kd);
		if(Encoder2Val<0)Encoder2Val = -Encoder2Val;
	    if(Encoder4Val<0)Encoder4Val = -Encoder4Val;
		Int2Char2(tx_frame.Encoder_L,Encoder2Val);
		Int2Char2(tx_frame.Encoder_R,Encoder4Val);
}
void TX_AttitudeFrame(void)
{
	tx_attitude.angle = Angle_Balance;//---pitch 由四元数得到
	tx_attitude.gyro = Gyro_Balance;//---y轴陀螺仪
	tx_attitude.turn = Gyro_Turn;//---z轴陀螺仪
	tx_attitude.accelz = Acceleration_Z;//---z轴加速度
	
	
}
