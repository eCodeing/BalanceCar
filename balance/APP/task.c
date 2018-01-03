#include  "task.h"
#define FLASH_SAVE_ADDR  0X08010034 	//设置FLASH 保存地址(必须为偶数，且所在扇区,要大于本代码所占用到的扇区.
										//否则,写操作的时候,可能会导致擦除整个扇区,从而引起部分程序丢失.引起死机.
void  BSP_Init(void)
{
	 delay_init(168);
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
     uart_init(115200);
	 UART3_init(9600);
	 TB6612_Init();
		   //PWM频率 84000/prc/arr=KHz
	//set pwm:10KHz
	 TIM3CH1CH2_PWM_Init(1050-1,8-1);
	 LED_Init();
	 BatVAL_Init();
	 EXTIx_Init();
	 SPI2_Init();
	NRF24L01_Init(40,RX);//2401选择40通道，接收模式
 	 //检测2401是否正常    
	 while(NRF24L01_Check()) 
	 {
	    LED0 = !LED0;
		LED1 = !LED1;
		delay_ms(300);
	 }
	 OLED_Init();
     IIC_Init();
     MPU6050_initialize();
	 DMP_Init();
	 printf("mpu6050 init ok\r\n");
//	 OLED_ShowString(0,0,"mpu6050 init ok",16);
	 OLED_ShowString(0,12,"Pitch:",12);
	 OLED_ShowString(0,0,"Battery:",12);
	 OLED_ShowString(0,24,"PwmL:",12);
	 OLED_ShowString(66,24,"PwmR:",12);
	 LED0 = 0;
	 LED1 = 0;
	 Encoder_Init_TIM2();
	 Encoder_Init_TIM4();
	 Timer5_Init(49,839);//0.5ms	
}

void  Task_OLED_Refresh(void)
{
		if(balance_angle.Pitch<0)	
			OLED_ShowNum(48,12,balance_angle.Pitch+360,3,12);
		else					        
			OLED_ShowNum(48,12,balance_angle.Pitch,3,12);	
		OLED_ShowInt(30,24,Encoder2Val,12);
		OLED_ShowInt(96,24,Encoder4Val,12);
		OLED_ShowFloat(48,0,(float)(BatVal*7.7f),12);
}

void  Task_CalculateBatVal(void)
{
		BatVal =ADCConvertedValue*3.3/4095;
}
void  Task_GetEulerAngle(ANGLE *eulerangle)
{
	eulerangle->Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;
	eulerangle->Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
	
}
void Task_StorePara(struct __Rc * rc)
{
	static float last_kp = 0.0f,last_ki = 0.0f,last_kd = 0.0f;
	if((last_kp != rc->kp) || (last_ki != rc->ki) || (last_kd != rc->kd))
	{
		STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)rc,8);
		last_kp = rc->kp;
		last_ki  = rc->ki;
		last_kd = rc->kd;
	}

}
void Task_ReadPara(struct __Rc *rc)
{
	STMFLASH_Read(FLASH_SAVE_ADDR,(u32*)rc,8);
}
//update kp ki kd
void Task_UpdatePara(struct __Rc *rc)
{
//	balance_kp = rc->kp;
//	balance_ki = rc->ki;
//	balance_kd = rc->kd;
//	velocity_kp = rc->kp;
//	velocity_ki = rc->ki;

//	velocity_kd = rc->kd;
//	turn_kp = rc->kd;
	Flag_Qian = rc->Dire_Front;
	Flag_Hou = rc->Dire_Back;
	Flag_Left = rc->Dire_Left;
	Flag_Right = rc->Dire_Right;
	Target_Velocity = rc->accel;
}
void Task_EncoderFilter(int16_t *Encoder2Val_t,int16_t *Encoder4Val_t)
{
	uint8_t i;
	int16_t temp_buf[40];
	for( i = 0;i<40;i+=2)
	{
		//读取左右两轮速度
	    (*Encoder2Val_t) =  Read_Encoder(2);
	    (*Encoder4Val_t) =  -Read_Encoder(4); 
		temp_buf[i] = (*Encoder2Val_t);
		temp_buf[i+1] = (*Encoder4Val_t);
	}
	for(i = 0;i<40;i+=2)
	{
		(*Encoder2Val_t) = temp_buf[i]/20;
		(*Encoder4Val_t) = temp_buf[i+1]/20;
	}
	
}
void  Task_1ms(void)//
{
	Read_DMP();//计算并更新姿态数据，四元数输出
	Get_Angle(2);//选择滤波方式
	Nrf_Connect();
	Task_UpdatePara(&Rc);
}
//方式2
//-40       -2
//-40.58   -2.31
//-40     -1.7
//-40     -1.5
//-40    -1.2
//-40    -1.1
void  Task_2ms(void)
{
//	Get_Angle(2);
	
}
void  Task_4ms(void)
{
	Task_CalculateBatVal();
}
	int pwm_balance = 0;
	int pwm_velocity = 0; 
	int pwm_turn = 0;
//int pwm_velocity_step = 0;
void  Task_5ms(void)
{
		int pwm_output_l , pwm_output_r = 0;
	    //读取左右两轮速度
	    Encoder2Val =  Read_Encoder(2);
	    Encoder4Val =  -Read_Encoder(4); 
		//计算直立环需要的pwm值
		pwm_balance = balance(Angle_Balance,Gyro_Balance); 
//		//平滑pwm_velocity
//	    pwm_velocity += pwm_velocity_step;
		//---更新左右两轮电机的PWM值
		pwm_output_l = pwm_balance-pwm_velocity+pwm_turn;
		pwm_output_r = pwm_balance-pwm_velocity-pwm_turn;
		pwm_moto_l = PWM_AmpLimit(pwm_output_l);
		pwm_moto_r = PWM_AmpLimit(pwm_output_r);
		//输出两轮电机PWM
	    Set_Pwm( pwm_moto_l,pwm_moto_r);
		Task_OLED_Refresh();
		OLED_Refresh_Gram();

}
void Task_10ms(void)
{
//	pwm_turn = turn(Encoder4Val,Encoder2Val,Gyro_Balance);	
}
void Task_50ms(void)
{
//	static int pwm_velocity_last = 0;
//	int pwm_velocity_temp = 0;
	//		//计算速度环需要的pwm值
		pwm_velocity = velocity(Encoder4Val,Encoder2Val);
	pwm_turn = turn(Encoder4Val,Encoder2Val,Gyro_Balance);
//	//计算本次和上次的差值
//	pwm_velocity_temp = pwm_velocity - pwm_velocity_last;
//	//更新步进值
//	pwm_velocity_step = pwm_velocity_temp/10;
//	//更新pwm_velocity_last变量值
//	pwm_velocity_last = pwm_velocity;
}
void Task_100ms(void)
{
	
}
void Task_200ms(void)
{
	
}
void  Task_500ms(void)
{
//	     uint8_t i = 0;
//		 TX_PackageFrame();
//	     for(i = 0;i< 32;i++)
//         printf("%c",*(((u8*)&tx_frame)+i));

}

