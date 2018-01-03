#include "key.h"
#include "delay.h"
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_KEY0_Pin|GPIO_KEY1_Pin|GPIO_KEY2_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;//��ͨ����
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin=GPIO_WK_UP_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;//��ͨ����
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;//����
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
}
//mode=1֧������ 
//
//mode=0��ʾ��֧������ �����������Ժ������Ӧ��һ��ɨ��
//��������:��ִ��KEY_Scan����ʱ,key_up��ʼֵΪ1��ʱ�޼�����
//key_upһֱΪ1����ֵΪ0
//���м�����ʱ�����������key_up=0,Ȼ�󷵻ذ��µļ�ֵ
//��ʱ���δ������CPU�ٴ����д˺���ʱ��Ϊkey_up������Ϊ0
//���Բ�����Ӧ�˰���
//��Ϊ��,���´�ɨ����Զ��ָ�key_upΪ1(��ʱ������,����������)
uint8_t KEY_Scan(uint8_t mode)
{
   static uint8_t key_up=1;//�����ɰ���־
   if(mode)key_up=1;
   if(key_up&&((KEY0==0)||(KEY1==0)||(KEY2==0)||(WK_UP==1)))
   {
        delay_ms(10);
	    key_up=0;
	    if(KEY0==0)return 1;
	    else if(KEY1==0)return 2;
	    else if(KEY2==0)return 3;
	    else if(WK_UP==1)return 4;
	   
   }
   else if((KEY0==1)&&(KEY1==1)&&(KEY2==1)&&(WK_UP==0))
   {
        key_up=1;
   }	   
	
   return 0;
}
