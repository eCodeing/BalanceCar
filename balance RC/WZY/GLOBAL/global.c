#include  "global.h"
//PWM���
int pwm_moto;
//ƽ�⳵�������
//float Angle_Balance;
float Gyro_Balance;
float balance_kp=-75;
float balance_kd=-0.115;//kp -75 kp -0.1

float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
//����3�������
uint8_t  Buffer_Uart3[BUFFER_UART3_LEN ];
uint16_t num_recv3;//��Ϊuart3���������ᳬ��512�ֽ�
//����2�������
uint8_t  Buffer_Uart2[BUFFER_UART2_LEN ];
uint16_t num_recv2;//��Ϊuart3���������ᳬ��512�ֽ�
//��ʱ��5�жϴ���
uint32_t Timer5_count = 0;
////��ص���,ADC�ɼ����
__IO uint16_t ADCConvertedValue;
float BatVal;
//���������
int Encoder2Val;
int Encoder4Val ;
//NRF24L01 ���
u8 tmp_buf[33];
TX_Data  tx_frame;
RX_Data  rx_frame;
//��־λ��¼���
u8 Flag_Stop=1,Flag_Show=0; //ֹͣ��־λ�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��
u8 txdata_isok = 0;
u8 txdata_isfinished = 0;
u8 rxdata_isfinished = 1;
//�����ʱ����
u8 Count_2ms,Count_10ms,Count_40ms,Count_250ms;
//��¼�������ŷ����
ANGLE  balance_angle;
//
struct _Rc Rc;				//ң��ͨ��  
