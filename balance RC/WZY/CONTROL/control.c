#include "control.h"
/**************************************************************************
�������ܣ�ֱ��PD����
��ڲ������Ƕȡ����ٶ�
����  ֵ��ֱ������PWM
��    �ߣ�ƽ��С��֮��
**************************************************************************/
int balance(float Angle,float Gyro)
{  
   float balance_Bias;
	 int balance_pwm;
	 balance_Bias=Angle-ZHONGZHI;       //===���ƽ��ĽǶ���ֵ �ͻ�е���
	 balance_pwm=balance_kp*balance_Bias+Gyro*balance_kd;   //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	 return balance_pwm;
}
/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
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
