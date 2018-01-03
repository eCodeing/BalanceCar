#include "control.h"
/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
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
//	 balance_pwm = Rc.kp*balance_Bias+Gyro*Rc.kd;
	return balance_pwm;
}
//速度环调试，PI控制
//返回值为计算得到的PWM值
int velocity(int encoder_left,int encoder_right)
{
	static float Encoder_Least = 0.0f,Encoder = 0.0f,Encoder_Integral = 0.0f,Movement;
	static float Velocity = 0.0f;
	  //=============遥控前进后退部分=======================// 
//	  if(Bi_zhang==1&&Flag_sudu==1)  Target_Velocity=45;                 //如果进入避障模式,自动进入低速模式
//    else 	                         Target_Velocity=90;                 
		if(1==Flag_Qian)    	Movement=-Target_Velocity/Flag_sudu;	         //===前进标志位置1 
		else if(1==Flag_Hou)	Movement=Target_Velocity/Flag_sudu;         //===后退标志位置1
	  else  Movement=0;	
//	  if(Bi_zhang==1&&Distance<500&&Flag_Left!=1&&Flag_Right!=1)        //避障标志位置1且非遥控转弯的时候，进入避障模式
//	  Movement=-Target_Velocity/Flag_sudu;
	//===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
	Encoder_Least = (encoder_left + encoder_right) - g_iCarSpeedSet;
	//低通滤波
	Encoder *= 0.8f; 
	Encoder += Encoder_Least*0.2f;
	//积分计算
//	Encoder_Integral += Encoder_Least;
	Encoder_Integral += Encoder;
	Encoder_Integral -= Movement;
	//积分限幅
	if(Encoder_Integral>10000)Encoder_Integral = 10000;
	if(Encoder_Integral<-10000)Encoder_Integral = -10000;
	//pi控制
	Velocity = Encoder*velocity_kp + Encoder_Integral*velocity_ki;
	return Velocity ;
}
int turn(int encoder_left,int encoder_right,float gyro)
{
//	float Turn,Bias;
//	Bias = gyro - 0;//目标角速度设为0，小车走直线
//	Turn = -Bias*turn_kp;
//	return Turn;
	static float Turn_Target,Turn,Encoder_temp,Turn_Convert=0.9,Turn_Count;
	float Turn_Amplitude=88/Flag_sudu,Kp=-42,Kd=0;     
	  //=============遥控左右旋转部分=======================//
  	if(1==Flag_Left||1==Flag_Right)                      //这一部分主要是根据旋转前的速度调整速度的起始速度，增加小车的适应性
		{
			if(++Turn_Count==1)
			Encoder_temp=myabs(encoder_left+encoder_right);
			Turn_Convert=50/Encoder_temp;
			if(Turn_Convert<0.6f)Turn_Convert=0.6f;
			if(Turn_Convert>3)Turn_Convert=3;
		}	
	  else
		{
			Turn_Convert=0.9;
			Turn_Count=0;
			Encoder_temp=0;
		}			
		if(1==Flag_Left)	           Turn_Target-=Turn_Convert;
		else if(1==Flag_Right)	     Turn_Target+=Turn_Convert; 
		else Turn_Target=0;
	
    if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;    //===转向速度限幅
	  if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;
//		if(Flag_Qian==1||Flag_Hou==1)  Kd=0.5;        
//		else Kd=0;   //转向的时候取消陀螺仪的纠正 有点模糊PID的思想
  	//=============转向PD控制器=======================//
		Turn=-Turn_Target*Kp -gyro*Kd;                 //===结合Z轴陀螺仪进行PD控制
	  return Turn;
}
//int velocity2(int16_t Encoder_Left,int16_t Encoder_Right)
//{
//	static float Encoder_New;//存储编码器最新值
//	static float Encoder_Old;//存储上一次编码器值
//	static float fDelta,fP,fI;
//	static float Encoder_Integral = 0.0f;
//	Encoder_New = (Encoder_Left + Encoder_Right)*0.5f;
//	Encoder_New = (Encoder_Old*0.2f + Encoder_New*0.8f);
//	Encoder_Old = Encoder_New;
//	fDelta = g_iCarSpeedSet - Encoder_New;
//	fP = fDelta*velocity_kp;
//	fI = fDelta*velocity_ki;
//	
//	Encoder_Integral += fI+fP;
//}
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
extern void  Task_GetEulerAngle(ANGLE *eulerangle);
/**************************************************************************
函数功能：获取角度 三种算法经过我们的调校，都非常理想 
入口参数：获取角度的算法 1：DMP  2：卡尔曼 3：互补滤波
返回  值：无
**************************************************************************/
void Get_Angle(u8 way)
{ 
	    float Accel_Y,Accel_X,Accel_Z,Gyro_Y,Gyro_Z;
//	   	Temperature=Read_Temperature();      //===读取MPU6050内置温度传感器数据，近似表示主板温度。
	    if(way==1)                           //===DMP的读取在数据采集中断提醒的时候，严格遵循时序要求
			{	
					Read_DMP();                      //===读取加速度、角速度、倾角
					Task_GetEulerAngle(&balance_angle);
					Angle_Balance=balance_angle.Pitch;             //===更新平衡倾角
					Gyro_Balance=gyro[1];            //===更新平衡角速度
					Gyro_Turn=gyro[2];               //===更新转向角速度
				  Acceleration_Z=accel[2];         //===更新Z轴加速度计
			}			
      else
      {
			Gyro_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_L);    //读取Y轴陀螺仪
			Gyro_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_L);    //读取Z轴陀螺仪
		  Accel_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_L); //读取X轴加速度计
	  	Accel_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_L); //读取Z轴加速度计
		  if(Gyro_Y>32768)  Gyro_Y-=65536;                       //数据类型转换  也可通过short强制类型转换
			if(Gyro_Z>32768)  Gyro_Z-=65536;                       //数据类型转换
	  	if(Accel_X>32768) Accel_X-=65536;                      //数据类型转换
		  if(Accel_Z>32768) Accel_Z-=65536;                      //数据类型转换
			Gyro_Balance=-Gyro_Y;                                  //更新平衡角速度
	   	Accel_Y=atan2(Accel_X,Accel_Z)*180/PI;                 //计算倾角	
		  Gyro_Y=Gyro_Y/16.4f;                                    //陀螺仪量程转换	
      if(way == 2)		  	Kalman_Filter(Accel_Y,-Gyro_Y);//卡尔曼滤波	
			else if(way == 3)   Yijielvbo(Accel_Y,-Gyro_Y);    //互补滤波
	    Angle_Balance=angle;                                   //更新平衡倾角
			Gyro_Turn=Gyro_Z;                                      //更新转向角速度
			Acceleration_Z=Accel_Z;                                //===更新Z轴加速度计	
		}
}
