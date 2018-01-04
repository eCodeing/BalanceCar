#ifndef   __TIMER_H
#define   __TIMER_H
#include "sys.h"
#include "global.h"
/*TIM2-7 TIM12-14挂载到APB1总线
 *定时器3使用
 *SystemInit函数已经将APB2 2分频 84M
 *     PCLK2 = HCLK/2 
 *   RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
 *SystemInit函数已经将APB1 4分频 42M
 *APB1时钟分频数为4,TIM3时钟频率为APB1 2倍 
 *    PCLK1 = HCLK / 4
 *   RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
 *   arr:自动重装载值 psc:预分频数
 * 定时器溢出时间Tout:((arr+1)*(psc+1))/Ft us
 * Ft:定时器工作频率 84MHz
 */
void  Timer3_Init(u16 arr,u16 psc);
void  Timer5_Init(u16 arr,u16 psc);
void  TIM3CH1CH2_PWM_Init(u16 arr,u16 psc);
void  PWM_AmpLimit(int pwm_temp);
#endif
