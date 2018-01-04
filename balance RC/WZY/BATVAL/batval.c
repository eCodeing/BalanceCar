#include "batval.h"

/***************************************************************************************
旋钮输出模拟量，需要AD转换得到数字量
左：PA4  ADC12_IN4
右：PA5  ADC12_IN5

ADC1通道4和通道5使用DMA方式
DMA数据流0通道0是ADC1
ADC1_DR=ADC1_BASE+DR_OFFSET(0x4C)
ADC1_BASE=(APB2PERIPH_BASE + 0x2000)
APB2PERIPH_BASE=(PERIPH_BASE + 0x00010000)
PERIPH_BASE=((uint32_t)0x40000000)
计算ADC1_DR=0x4001204C
    ADC3_DR=0x4001224C
***************************************************************************************/
#define   ADC1_DR_ADDRESS    ((uint32_t)0x4001204C)

void BatVAL_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_DMA2,ENABLE);//PA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//ADC1时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AN;//模拟输入
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
    DMA_InitStructure.DMA_Channel=DMA_Channel_0;
    DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)ADC1_DR_ADDRESS;
    //ADCConvertedValue[0]存放ADC1通道4转换结果
    DMA_InitStructure.DMA_Memory0BaseAddr=(uint32_t)&ADCConvertedValue;	
    DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize=2;
    DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;	
    DMA_InitStructure.DMA_Priority=DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode=DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold=DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst=DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0,&DMA_InitStructure);
    DMA_Cmd(DMA2_Stream0,ENABLE);
	//独立模式
	ADC_CommonInitStructure.ADC_Mode=ADC_Mode_Independent;
	//2个采样阶段延迟5个时钟周期
	ADC_CommonInitStructure.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;
	//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
	ADC_CommonInitStructure.ADC_Prescaler=ADC_Prescaler_Div4;
	//DMA失能
	ADC_CommonInitStructure.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式	
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//开启连续转换
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
    ADC_InitStructure.ADC_NbrOfConversion = 2;//2个转换在规则序列中 也就是只转换规则序列1 
    ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
	ADC_DMACmd(ADC1,ENABLE);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_144Cycles );
 //   ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_15Cycles );	
	//转换完成使能DMA请求
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
	
   	ADC_Cmd(ADC1, ENABLE);//开启AD转换器
	ADC_SoftwareStartConv(ADC1);
	
}
