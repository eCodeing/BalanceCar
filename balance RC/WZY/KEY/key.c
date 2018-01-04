#include "key.h"
#include "delay.h"
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_KEY0_Pin|GPIO_KEY1_Pin|GPIO_KEY2_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;//普通输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin=GPIO_WK_UP_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;//普通输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;//下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
}
//mode=1支持连按 
//
//mode=0表示不支持连按 即必须松手以后才能响应下一次扫描
//分析如下:在执行KEY_Scan函数时,key_up初始值为1此时无键按下
//key_up一直为1返回值为0
//当有键按下时如果不是误按则key_up=0,然后返回按下的键值
//此时如果未松手则当CPU再次运行此函数时因为key_up变量仍为0
//所以不会响应此按键
//若为误按,则下次扫描后自动恢复key_up为1(此时已松手,不松手则不误按)
uint8_t KEY_Scan(uint8_t mode)
{
   static uint8_t key_up=1;//按键松按标志
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
