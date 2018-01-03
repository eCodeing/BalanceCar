#include "batval.h"

/***************************************************************************************
��ť���ģ��������ҪADת���õ�������
��PA4  ADC12_IN4
�ң�PA5  ADC12_IN5

ADC1ͨ��4��ͨ��5ʹ��DMA��ʽ
DMA������0ͨ��0��ADC1
ADC1_DR=ADC1_BASE+DR_OFFSET(0x4C)
ADC1_BASE=(APB2PERIPH_BASE + 0x2000)
APB2PERIPH_BASE=(PERIPH_BASE + 0x00010000)
PERIPH_BASE=((uint32_t)0x40000000)
����ADC1_DR=0x4001204C
    ADC3_DR=0x4001224C
***************************************************************************************/
#define   ADC1_DR_ADDRESS    ((uint32_t)0x4001204C)

void BatVAL_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_DMA2,ENABLE);//PAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//ADC1ʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AN;//ģ������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
    DMA_InitStructure.DMA_Channel=DMA_Channel_0;
    DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)ADC1_DR_ADDRESS;
    //ADCConvertedValue[0]���ADC1ͨ��4ת�����
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
	//����ģʽ
	ADC_CommonInitStructure.ADC_Mode=ADC_Mode_Independent;
	//2�������׶��ӳ�5��ʱ������
	ADC_CommonInitStructure.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;
	//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
	ADC_CommonInitStructure.ADC_Prescaler=ADC_Prescaler_Div4;
	//DMAʧ��
	ADC_CommonInitStructure.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ	
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//��������ת��
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
    ADC_InitStructure.ADC_NbrOfConversion = 2;//2��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	ADC_DMACmd(ADC1,ENABLE);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_144Cycles );
 //   ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_15Cycles );	
	//ת�����ʹ��DMA����
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
	
   	ADC_Cmd(ADC1, ENABLE);//����ADת����
	ADC_SoftwareStartConv(ADC1);
	
}
