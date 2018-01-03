#include "control.h"
/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
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
//	 balance_pwm = Rc.kp*balance_Bias+Gyro*Rc.kd;
	return balance_pwm;
}
//�ٶȻ����ԣ�PI����
//����ֵΪ����õ���PWMֵ
int velocity(int encoder_left,int encoder_right)
{
	static float Encoder_Least = 0.0f,Encoder = 0.0f,Encoder_Integral = 0.0f,Movement;
	static float Velocity = 0.0f;
	  //=============ң��ǰ�����˲���=======================// 
//	  if(Bi_zhang==1&&Flag_sudu==1)  Target_Velocity=45;                 //����������ģʽ,�Զ��������ģʽ
//    else 	                         Target_Velocity=90;                 
		if(1==Flag_Qian)    	Movement=-Target_Velocity/Flag_sudu;	         //===ǰ����־λ��1 
		else if(1==Flag_Hou)	Movement=Target_Velocity/Flag_sudu;         //===���˱�־λ��1
	  else  Movement=0;	
//	  if(Bi_zhang==1&&Distance<500&&Flag_Left!=1&&Flag_Right!=1)        //���ϱ�־λ��1�ҷ�ң��ת���ʱ�򣬽������ģʽ
//	  Movement=-Target_Velocity/Flag_sudu;
	//===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
	Encoder_Least = (encoder_left + encoder_right) - g_iCarSpeedSet;
	//��ͨ�˲�
	Encoder *= 0.8f; 
	Encoder += Encoder_Least*0.2f;
	//���ּ���
//	Encoder_Integral += Encoder_Least;
	Encoder_Integral += Encoder;
	Encoder_Integral -= Movement;
	//�����޷�
	if(Encoder_Integral>10000)Encoder_Integral = 10000;
	if(Encoder_Integral<-10000)Encoder_Integral = -10000;
	//pi����
	Velocity = Encoder*velocity_kp + Encoder_Integral*velocity_ki;
	return Velocity ;
}
int turn(int encoder_left,int encoder_right,float gyro)
{
//	float Turn,Bias;
//	Bias = gyro - 0;//Ŀ����ٶ���Ϊ0��С����ֱ��
//	Turn = -Bias*turn_kp;
//	return Turn;
	static float Turn_Target,Turn,Encoder_temp,Turn_Convert=0.9,Turn_Count;
	float Turn_Amplitude=88/Flag_sudu,Kp=-42,Kd=0;     
	  //=============ң��������ת����=======================//
  	if(1==Flag_Left||1==Flag_Right)                      //��һ������Ҫ�Ǹ�����תǰ���ٶȵ����ٶȵ���ʼ�ٶȣ�����С������Ӧ��
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
	
    if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;    //===ת���ٶ��޷�
	  if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;
//		if(Flag_Qian==1||Flag_Hou==1)  Kd=0.5;        
//		else Kd=0;   //ת���ʱ��ȡ�������ǵľ��� �е�ģ��PID��˼��
  	//=============ת��PD������=======================//
		Turn=-Turn_Target*Kp -gyro*Kd;                 //===���Z�������ǽ���PD����
	  return Turn;
}
//int velocity2(int16_t Encoder_Left,int16_t Encoder_Right)
//{
//	static float Encoder_New;//�洢����������ֵ
//	static float Encoder_Old;//�洢��һ�α�����ֵ
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
extern void  Task_GetEulerAngle(ANGLE *eulerangle);
/**************************************************************************
�������ܣ���ȡ�Ƕ� �����㷨�������ǵĵ�У�����ǳ����� 
��ڲ�������ȡ�Ƕȵ��㷨 1��DMP  2�������� 3�������˲�
����  ֵ����
**************************************************************************/
void Get_Angle(u8 way)
{ 
	    float Accel_Y,Accel_X,Accel_Z,Gyro_Y,Gyro_Z;
//	   	Temperature=Read_Temperature();      //===��ȡMPU6050�����¶ȴ��������ݣ����Ʊ�ʾ�����¶ȡ�
	    if(way==1)                           //===DMP�Ķ�ȡ�����ݲɼ��ж����ѵ�ʱ���ϸ���ѭʱ��Ҫ��
			{	
					Read_DMP();                      //===��ȡ���ٶȡ����ٶȡ����
					Task_GetEulerAngle(&balance_angle);
					Angle_Balance=balance_angle.Pitch;             //===����ƽ�����
					Gyro_Balance=gyro[1];            //===����ƽ����ٶ�
					Gyro_Turn=gyro[2];               //===����ת����ٶ�
				  Acceleration_Z=accel[2];         //===����Z����ٶȼ�
			}			
      else
      {
			Gyro_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_L);    //��ȡY��������
			Gyro_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_L);    //��ȡZ��������
		  Accel_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_L); //��ȡX����ٶȼ�
	  	Accel_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_L); //��ȡZ����ٶȼ�
		  if(Gyro_Y>32768)  Gyro_Y-=65536;                       //��������ת��  Ҳ��ͨ��shortǿ������ת��
			if(Gyro_Z>32768)  Gyro_Z-=65536;                       //��������ת��
	  	if(Accel_X>32768) Accel_X-=65536;                      //��������ת��
		  if(Accel_Z>32768) Accel_Z-=65536;                      //��������ת��
			Gyro_Balance=-Gyro_Y;                                  //����ƽ����ٶ�
	   	Accel_Y=atan2(Accel_X,Accel_Z)*180/PI;                 //�������	
		  Gyro_Y=Gyro_Y/16.4f;                                    //����������ת��	
      if(way == 2)		  	Kalman_Filter(Accel_Y,-Gyro_Y);//�������˲�	
			else if(way == 3)   Yijielvbo(Accel_Y,-Gyro_Y);    //�����˲�
	    Angle_Balance=angle;                                   //����ƽ�����
			Gyro_Turn=Gyro_Z;                                      //����ת����ٶ�
			Acceleration_Z=Accel_Z;                                //===����Z����ٶȼ�	
		}
}
