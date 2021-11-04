#include "timer.h"

extern vu16 USART2_RX_STA;

//�߼���ʱ�� 1  ��ʼ������
void tim1_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = (100*10)-1;				//����ֵ	100ms
	TIM_TimeBaseStructure.TIM_Prescaler = 16800-1;				//Ԥ��Ƶֵ��Ҳ�������Ϊ��һ�η�Ƶ	�߼���ʱ��16800
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM1,TIM_IT_Update , ENABLE);
	
	TIM_Cmd(TIM1, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;		//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);	
}

//ͨ�ö�ʱ�� 2	��ʼ������
void tim2_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = (200*10)-1;				//����ֵ	200ms
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;				//Ԥ��Ƶֵ��Ҳ�������Ϊ��һ�η�Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update , ENABLE);
	
	TIM_Cmd(TIM2, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;		//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);	
}

//ͨ�ö�ʱ�� 3	��ʼ������
void tim3_init(void)
{
	//ʹ�ܶ�ʱ��3��Ӳ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	
	//���ö�ʱ��3��������ʱʱ��
	//��ʱ��3��Ӳ��ʱ��=84MHz/8400=10000Hz
	//ֻҪ��ʱ��3����10000�μ���������1��ʱ��ĵ���
	//ֻҪ��ʱ��3����10�μ���������1����ʱ��ĵ���
	TIM_TimeBaseStructure.TIM_Period = (500*10)-1;				//����ֵ	500ms
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;				//Ԥ��Ƶֵ��Ҳ�������Ϊ��һ�η�Ƶ
	//TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//F407��û��ʱ�ӷ�Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	//���ö�ʱ��3���ж�
	TIM_ITConfig(TIM3,TIM_IT_Update , ENABLE);
	
	//������ʱ��3
	TIM_Cmd(TIM3, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				//��ʱ��3���жϺ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;		//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//NVICʹ�ܶ�ʱ��3�ж�����ͨ��
	NVIC_Init(&NVIC_InitStructure);	
}

//�߼���ʱ�� 8	��ʼ������
void tim8_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = (2000*10)-1;				//����ֵ	2000ms
	TIM_TimeBaseStructure.TIM_Prescaler = 16800-1;				//Ԥ��Ƶֵ��Ҳ�������Ϊ��һ�η�Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM8,TIM_IT_Update , ENABLE);
	
	TIM_Cmd(TIM8, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;		//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}


//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ42M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM7_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//TIM7ʱ��ʹ��    
	
	//��ʱ��TIM7��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM7�ж�,��������ж�

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}

//�߼���ʱ�� 1 10  �жϷ�����
void TIM1_UP_TIM10_IRQHandler(void)
{
	//����־λ
	if(TIM_GetITStatus(TIM1,TIM_IT_Update) == SET)
	{
		PFout(9)^=1;
		//��ձ�־λ
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	
	}
}
//ͨ�ö�ʱ�� 2  �жϷ�����
void TIM2_IRQHandler(void)
{
	//����־λ
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{

		PFout(10)^=1;
		//��ձ�־λ
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	
	}	
}

//ͨ�ö�ʱ�� 3  �жϷ�����
void TIM3_IRQHandler(void)
{
	//����־λ
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET)
	{
		PEout(13)^=1;
		//��ձ�־λ
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	
	}
}

//�߼���ʱ�� 8 13  �жϷ�����
void TIM8_UP_TIM13_IRQHandler(void)
{
	//����־λ
	if(TIM_GetITStatus(TIM8,TIM_IT_Update) == SET)
	{
		PEout(14)^=1;
		//��ձ�־λ
		TIM_ClearITPendingBit(TIM8,TIM_IT_Update);
	
	}	
}

//��ʱ��7�жϷ������		    
void TIM7_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   
		USART2_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //���TIM7�����жϱ�־    
		TIM_Cmd(TIM7, DISABLE);  //�ر�TIM7 
	}	    
}
 
