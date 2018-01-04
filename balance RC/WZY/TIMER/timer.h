#ifndef   __TIMER_H
#define   __TIMER_H
#include "sys.h"
#include "global.h"
/*TIM2-7 TIM12-14���ص�APB1����
 *��ʱ��3ʹ��
 *SystemInit�����Ѿ���APB2 2��Ƶ 84M
 *     PCLK2 = HCLK/2 
 *   RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
 *SystemInit�����Ѿ���APB1 4��Ƶ 42M
 *APB1ʱ�ӷ�Ƶ��Ϊ4,TIM3ʱ��Ƶ��ΪAPB1 2�� 
 *    PCLK1 = HCLK / 4
 *   RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
 *   arr:�Զ���װ��ֵ psc:Ԥ��Ƶ��
 * ��ʱ�����ʱ��Tout:((arr+1)*(psc+1))/Ft us
 * Ft:��ʱ������Ƶ�� 84MHz
 */
void  Timer3_Init(u16 arr,u16 psc);
void  Timer5_Init(u16 arr,u16 psc);
void  TIM3CH1CH2_PWM_Init(u16 arr,u16 psc);
void  PWM_AmpLimit(int pwm_temp);
#endif
