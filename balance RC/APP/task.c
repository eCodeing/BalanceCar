#include  "task.h"
#include  "hmi.h"
void  BSP_Init(void)
{
	 delay_init(168);
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
     uart_init(115200);
	 
	 
     UART2_init(9600);
	 
	 LED_Init();
	 SPI2_Init();
	 NRF24L01_Init(40,TX);//2401ѡ��40ͨ��������ģʽ
 	 //���2401�Ƿ�����    
	 while(NRF24L01_Check()) 
	 {
	    LED0 = !LED0;
		LED1 = !LED1;
		delay_ms(300);
	 }
//	 OLED_Init();
     IIC_Init();
     MPU6050_initialize();
	 DMP_Init();
	 printf("mpu6050 init ok\r\n");
//	 OLED_ShowString(0,0,"mpu6050 init ok",16);
//	 OLED_ShowString(0,32,"Pitch:",16);
//	 OLED_ShowString(0,16,"Battery:",16);
	 LED0 = 0;
	 LED1 = 0;
//	Encoder_Init_TIM2();
//	Encoder_Init_TIM4();
	 Timer5_Init(199,839);//2ms
//	NRF24L01_TX_Mode();
}

void  Task_OLED_Refresh(void)
{
		if(balance_angle.Pitch<0)	
			OLED_ShowNum(48,32,balance_angle.Pitch+360,3,12);
		else					        
			OLED_ShowNum(48,32,balance_angle.Pitch,3,12);	
		OLED_ShowFloat(80,32,(float)pwm_moto,12);
		OLED_ShowFloat(64,16,(float)(BatVal*7.7f),16);
}

void  Task_CalculateBatVal(void)
{
		BatVal =ADCConvertedValue*3.3/4095;
}
static void  Task_GetEulerAngle(ANGLE *eulerangle)
{
	eulerangle->Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;
	eulerangle->Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
	
}
//void  Task_1ms(void)//
//{
//	Read_DMP();//���㲢������̬���ݣ���Ԫ�����
//	Nrf_Connect();
//}
//void  Task_2ms(void)
//{

//}
//void  Task_4ms(void)
//{
//		Task_CalculateBatVal();
//	    Task_GetEulerAngle(&balance_angle);

//}
//void  Task_5ms(void)
//{
//	    int pwm_temp = 0;
////	 Encoder2Val =  Read_Encoder(2);
////	 Encoder4Val =  Read_Encoder(4); 
//		Gyro_Balance = gyro[1];
//		pwm_temp = balance(balance_angle.Pitch,Gyro_Balance); 
//		PWM_AmpLimit(pwm_temp);
////	    Set_Pwm( pwm_moto,pwm_moto);
//		Task_OLED_Refresh();
//		OLED_Refresh_Gram();
//}

u8 tx_temp[32]={'i','l','o','v','e'};
/******************************************************************************
����ԭ�ͣ�	void Task_500HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ500HZ����
*******************************************************************************/ 
void Task_500HZ(void)
{
	
	Nrf_Connect();//NRF24L01���Ӻ���
//	Rc_Filter(&Rc,&Filter_Rc);//����ң��ָ��+�˲�
//	HMI_DisplaySine();

}
extern gUART_FIFO gUART2;
/******************************************************************************
����ԭ�ͣ�	void Task_100HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ100HZ����
*******************************************************************************/ 
void Task_100HZ(void)
{
	uint8_t temp;
	NRF_Send_TX(tx_temp,32); 
//	Print_MSP_RC('<');//����ң���źŸ��ɿ�
	if(Get_From_FIFO(&gUART2,&temp)==0)
	{
//		UART2_SendData("i have receive!\r\n");
		LED0 = !LED0;
		LED1 = !LED1;
	}
	
}

/******************************************************************************
����ԭ�ͣ�	void Task_25HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ25HZ����
*******************************************************************************/ 
void Task_25HZ(void)
{
//	Print_MSP_RC('>');//����ң���źŸ���λ��
//	LED_Show();//LEDָʾ����˸
//	Print_MSP_MOTOR();//���͵��ת�ٸ���λ��
	
}
uint8_t processbar;
/******************************************************************************
����ԭ�ͣ�	void Task_4HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ4HZ����
*******************************************************************************/ 
void Task_4HZ(void)
{
//	static uint8_t turn_count;
//	
//	turn_count++;
//	switch(turn_count)
//	{
//		case 1: Print_MSP_ANALOG();		break;
//		case 2: Print_MSP_IDENT();		break;
//		case 3: Print_MSP_ANALOG();		break;				
//		case 4: Print_MSP_MOTOR_PINS();	
//				turn_count=0; 			break;
//	if(processbar>=100)processbar = 0;
//	processbar++;
	HMI_DisplayInfo (); 
}

