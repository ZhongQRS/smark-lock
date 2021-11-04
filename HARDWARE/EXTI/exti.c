#include "exti.h"

//�ж���9 ��ʼ������
void EXTI9_Init(void)
{
	//ʹ��SYSCFGʱ�ӣ� ʹ���ж�/�¼�����������ʹ�����ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	//��GPIO G��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9;  	//����6
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN;	//����ģʽ
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_DOWN;	//����
	GPIO_Init(GPIOG,&GPIO_InitStruct);//��ʼ��IO��PG9Ϊ����
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource9);	//PG9
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
}
