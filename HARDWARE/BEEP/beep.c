#include "beep.h"

//��������ʼ��
void BEEP_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//�򿪶˿�ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//���ö˿�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				//����8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;		//����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		//����������
	GPIO_Init(GPIOF, &GPIO_InitStructure);					//�˿ڳ�ʼ��
}
