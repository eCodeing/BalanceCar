#ifndef __KEY_H
#define __KEY_H
#include "sys.h"
#define  GPIO_KEY0_Port       GPIOE
#define  GPIO_KEY1_Port       GPIOE
#define  GPIO_KEY2_Port       GPIOE
#define  GPIO_WK_UP_Port      GPIOA
#define  GPIO_KEY0_Pin        GPIO_Pin_4
#define  GPIO_KEY1_Pin        GPIO_Pin_3
#define  GPIO_KEY2_Pin        GPIO_Pin_2
#define  GPIO_WK_UP_Pin       GPIO_Pin_0

#define  KEY0   GPIO_ReadInputDataBit(GPIO_KEY0_Port,GPIO_KEY0_Pin) 
#define  KEY1   GPIO_ReadInputDataBit(GPIO_KEY1_Port,GPIO_KEY1_Pin ) 
#define  KEY2   GPIO_ReadInputDataBit(GPIO_KEY2_Port,GPIO_KEY2_Pin) 
#define  WK_UP  GPIO_ReadInputDataBit(GPIO_WK_UP_Port,GPIO_WK_UP_Pin) 


void KEY_Init(void);
uint8_t KEY_Scan(uint8_t mode);
#endif

