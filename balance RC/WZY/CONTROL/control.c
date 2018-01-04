#include "control.h"
/**************************************************************************
函数功能：直立PD控制
入口参数：角度、角速度
返回  值：直立控制PWM
作    者：平衡小车之家
**************************************************************************/
int balance(float Angle,float Gyro)
{  
   float balance_Bias;
	 int balance_pwm;
	 balance_Bias=Angle-ZHONGZHI;       //===求出平衡的角度中值 和机械相关
	 balance_pwm=balance_kp*balance_Bias+Gyro*balance_kd;   //===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
	 return balance_pwm;
}
/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int moto1,int moto2)
{
    	if(moto1<0)			    
		{
			moto1 = -moto1;
             Moto_LeftRun_Backward(moto1);
		}//left
		else 	        
		{
            Moto_LeftRun_Forward(moto1);
		}
    	if(moto2<0)			    
		{
			moto2 = -moto2;
             Moto_RightRun_Backward(moto2);
		}//left
		else 	        
		{
            Moto_RightRun_Forward(moto2);
		}
}
