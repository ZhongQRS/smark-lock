#include "keyboard.h"

//PB7 PA4 PG15 PC7  ��	����
//PC9 PB6 PE6 PA8	��	���

extern OS_Q		g_queue;			//��Ϣ����
OS_ERR err;

void KEYBOARD_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA , ENABLE);	//ʹ��GPIOB��GPIOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOC , ENABLE);	//ʹ��GPIOG��GPIOCʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);			//ʹ��GPIOE

	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_7;  				//PB7
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;			    //��ͨ����ģʽ��
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//�������������LED��Ҫ��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		    //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;				    //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_4;  	//PA4
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_15;  	//PG15
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_7;  	//PC7
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 ;		//PC9
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;		//���ģʽ
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);						//��ʼ��GPIOF�������õ�����д��Ĵ���
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 ;		//PB6
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 ;		//PE6
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 ;		//PA8
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	
	PCout(9)=1;
	PBout(6)=1;
	PEout(6)=1;
	PAout(8)=1;
}


void set_one(void)
{
	PCout(9)=1;
	PBout(6)=1;
	PEout(6)=1;
	PAout(8)=1;
}



//PB7 PA4 PG15 PC7 ��
//PC9 PB6 PE6 PA8	��
void KEYBOARD_Num(void)
{
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7))
	{
		while(KeyBOARD_Scan(GPIOB,GPIO_Pin_7))//��1�� PB7
		{
			delay_ms(20);
			PCout(9)=0;
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 0 )
			{
				//1
				//printf("1\n");
				OSQPost(&g_queue,"1",strlen("1"),OS_OPT_POST_FIFO,&err);	//������Ϣ
				set_one();
				break;
			}
			PBout(6)=0;
			if(PBin(7)==0)
			{
				//4
				//printf("4\n");
				OSQPost(&g_queue,"4",strlen("4"),OS_OPT_POST_FIFO,&err);
				set_one();
				break;
			}
			PEout(6)=0;
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 0 )
			{
				//7
				//printf("7\n");
				OSQPost(&g_queue,"7",strlen("7"),OS_OPT_POST_FIFO,&err);
				set_one();
				break;
			}
			
			PAout(8)=0;
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 0 )
			{
				//��
				//printf("*\n");
				OSQPost(&g_queue,"*",strlen("*"),OS_OPT_POST_FIFO,&err);
				set_one();
				break;
			}				
		}
		while(KeyBOARD_Scan(GPIOB,GPIO_Pin_7));//��0��
	}
	
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4))
	{
		while(KeyBOARD_Scan(GPIOA,GPIO_Pin_4))//��1�� PB7
		{
			delay_ms(20);
			PCout(9)=0;
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) == 0 )
			{
				//2
				//printf("2\n");
				OSQPost(&g_queue,"2",strlen("2"),OS_OPT_POST_FIFO,&err);
				set_one();
				break;
			}
			PBout(6)=0;
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) == 0 )
			{
				//5
				//printf("5\n");
				OSQPost(&g_queue,"5",strlen("5"),OS_OPT_POST_FIFO,&err);
				set_one();
				break;
			}
			PEout(6)=0;
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) == 0 )
			{
				//8
				//printf("8\n");
				OSQPost(&g_queue,"8",strlen("8"),OS_OPT_POST_FIFO,&err);
				set_one();
				break;
			}
			
			PAout(8)=0;
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) == 0 )
			{
				//0
				//printf("0\n");
				OSQPost(&g_queue,"0",strlen("0"),OS_OPT_POST_FIFO,&err);
				set_one();
				break;
			}				
		}
		while(KeyBOARD_Scan(GPIOA,GPIO_Pin_4));//��1�� PB7
	}
	
	if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_15))
	{
		while(KeyBOARD_Scan(GPIOG,GPIO_Pin_15))//��1�� PB7
		{
			delay_ms(20);
			PCout(9)=0;
			if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_15) == 0 )
			{
				//3
				//printf("3\n");
				OSQPost(&g_queue,"3",strlen("3"),OS_OPT_POST_FIFO,&err);
				set_one();
				break;
			}
			PBout(6)=0;
			if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_15) == 0 )
			{
				//6
				//printf("6\n");
				OSQPost(&g_queue,"6",strlen("6"),OS_OPT_POST_FIFO,&err);
				set_one();
				break;
			}
			PEout(6)=0;
			if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_15) == 0 )
			{
				//9
				//printf("9\n");
				OSQPost(&g_queue,"9",strlen("9"),OS_OPT_POST_FIFO,&err);
				set_one();
				break;
			}
			
			PAout(8)=0;
			if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_15) == 0 )
			{
				//#
				//printf("#\n");
				OSQPost(&g_queue,"#",strlen("#"),OS_OPT_POST_FIFO,&err);
				set_one();
				break;
			}				
		}
		while(KeyBOARD_Scan(GPIOG,GPIO_Pin_15));//��1�� PB7
	}

	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7))
	{
		while(KeyBOARD_Scan(GPIOC,GPIO_Pin_7))//��1�� PB7
		{
			delay_ms(20);
			PCout(9)=0;
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7) == 0 )
			{
				//A
				//printf("A\n");
				OSQPost(&g_queue,"A",strlen("A"),OS_OPT_POST_FIFO,&err);
				set_one();
				break;
			}
			PBout(6)=0;
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7) == 0 )
			{
				//B
				//printf("B\n");
				OSQPost(&g_queue,"B",strlen("B"),OS_OPT_POST_FIFO,&err);
				set_one();
				break;
			}
			PEout(6)=0;
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7) == 0 )
			{
				//C
				//printf("C\n");
				OSQPost(&g_queue,"C",strlen("C"),OS_OPT_POST_FIFO,&err);
				set_one();
				break;
			}
			
			PAout(8)=0;
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7) == 0 )
			{
				//D
				//printf("D\n");
				OSQPost(&g_queue,"D",strlen("D"),OS_OPT_POST_FIFO,&err);
				set_one();
				break;
			}				
		}
		while(KeyBOARD_Scan(GPIOC,GPIO_Pin_7));//��1�� PB7
	}

}


/* ����Ƿ��а������� */
u8 KeyBOARD_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
{			
		/*����Ƿ��а������� */
   	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON ) 
	  {	   
			 /*��ʱ����*/
			delay_ms(30);		
			if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
			{	 
				return 	KEY_ON;	 
			}
			else
				return KEY_OFF;
		}
	else
		return KEY_OFF;
}
