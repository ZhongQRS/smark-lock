#include "lock.h" 
    
//LOCK IO��ʼ��	PD6
void LOCK_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);			//ʹ��GPIODʱ��

  //GPIOD6��ʼ������ 
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;					//��ӦIO��	6
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;			   		//��ͨ����ģʽ��
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		    //100MHz
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;				    //����
  GPIO_Init(GPIOD, &GPIO_InitStructure);						//��ʼ��GPIOD�������õ�����д��Ĵ���
}

void LOCK_ON(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_6; 		//����
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;	//���/����
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;    //����
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;     //����
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz; //�ٶ�
	
	GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void LOCK_OFF(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_6; 		//����
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN;	//���/����
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;    //����
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;     //����
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz; //�ٶ�
	
	GPIO_Init(GPIOD, &GPIO_InitStruct);
}


