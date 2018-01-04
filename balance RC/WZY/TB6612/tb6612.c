#include  "tb6612.h"
//#include  "control.h"
/*
		IN1 IN2  PWM  STBY  OUT1  OUT2    Mode
		H    H     H/L     H        L          L          short brake
		L     H    H         H       L          H         CCW
		L     H    L         H        L          L          short brake
		H     L    H         H       H          L          CW
		H     L    L          H       L          L           short brake
		L      L    H         H       OF       OF       stop
		H/L H/L  H/L      L        OF       OF       Standby

*/
void  TB6612_Init(void)
{
		  GPIO_InitTypeDef GPIO_InitStructure;
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC,ENABLE);
			//chA o1---o2
			GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//普通输出
			GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//推挽输出
			GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//上拉
			GPIO_Init(GPIOC,&GPIO_InitStructure);
      GPIO_ResetBits(GPIOC,GPIO_Pin_11|GPIO_Pin_12);	
      //chB o3---o4
			GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//普通输出
			GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//推挽输出
			GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//上拉
			GPIO_Init(GPIOB,&GPIO_InitStructure);
      GPIO_ResetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);		    
}

void  Moto_RightRun_Forward(uint32_t PWM_R)
{
//	 if(PWM_R<PWM_MAX)
	TIM_SetCompare1(TIM3,PWM_R);
	 GPIO_SetBits(GPIOC,GPIO_Pin_11);
	 GPIO_ResetBits(GPIOC,GPIO_Pin_12);//right
}
void  Moto_RightRun_Backward(uint32_t PWM_R)
{
//	 if(PWM_R<PWM_MAX)
	TIM_SetCompare1(TIM3,PWM_R);
	 GPIO_SetBits(GPIOC,GPIO_Pin_12);
	 GPIO_ResetBits(GPIOC,GPIO_Pin_11);//right
}
void  Moto_LeftRun_Forward(uint32_t PWM_L)
{
//	if(PWM_L<PWM_MAX)
	TIM_SetCompare2(TIM3,PWM_L);
    GPIO_SetBits(GPIOB,GPIO_Pin_9);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);//left
}
void  Moto_LeftRun_Backward(uint32_t PWM_L)
{
//	if(PWM_L<PWM_MAX)
	TIM_SetCompare2(TIM3,PWM_L);
    GPIO_SetBits(GPIOB,GPIO_Pin_8);
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);//left
}

