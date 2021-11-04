#include "sys.h"
#include "mifare522.h"	
#include "usart.h"
#include "delay.h"


//��ȡ�豸������
u8 readCmd[6]={0x06,0x01,0x41,0x00,0xB9,0x03};
//�������
u8 requestCmd[7]={0x07,0x02,0x41,0x01,0x52,0x00,0x03};
//����ײ
u8 piccAnticollCmd[8]={0x08,0x02,0x42,0x02,0x93,0x00,0x00,0x03};

//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART3_RX_STA=0;       //����״̬���	

//��ʼ��IO ����1 
//bound:������
void uart3_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ�� ʹ��USART1ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	//����3��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART3
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10��GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PB10��PB11

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure); //��ʼ������3

	
  USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���2 
	
	//USART_ClearFlag(USART3, USART_FLAG_TC);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//���������ж�

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3 ��ֵԽС���ȼ�Խ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3; //�����ȼ�3,��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ�����


}


void USART3_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�
	{
		Res =USART_ReceiveData(USART3);//(USART3->DR);	//��ȡ���յ�������
		if(USART3_RX_STA==0)
		{
			USART3_RX_BUF[0]=Res;
			USART3_RX_STA++;
		}
		else
		{		
				USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
				USART3_RX_STA++;
				if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	 
				if(USART3_RX_BUF[0]==USART3_RX_STA)USART3_RX_STA|=0x8000;				
			
		}	  
	} 
}

void USART3_SendByte(u8 Data)
{
   while (!(USART3->SR & USART_FLAG_TXE));	 // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		USART_SendData(USART3, Data);	 
   
}

void print3RX(u8 * data,u16 nub)			     // ��ӡ�����ַ���
{
	u16 i;
	for (i=0; i<nub; i++){
		USART3_SendByte(data[i]);
	}
}

void UART3_Send_Enter(void)
{
    USART3_SendByte(0x0d);
    USART3_SendByte(0x0a);
}

/*����У���*/
u8 CalBCC(u8 *buf, int n)
{
	u8 i;
	u8 bcc=0;
	for(i = 0; i < n; i++)
	{
		bcc ^= *(buf+i);
	}
	return (~bcc);
}

/**********************************
//������void mifare522Read() 
//�������ݣ�06 01 41 00 B9 03
//����ֵ��14 01 00 0E 52 43 35 32 32 20 56 31 2E 32 30 31 38 00 A2 03  
**************************************/
u8 mifare522ReadDevice(void)
{
	u16 len,t;
	print3RX(readCmd,6);	
	while(!(USART3_RX_STA&0x8000));
	if(USART3_RX_STA&0x8000)
	{					   
			len=USART3_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			if(USART3_RX_BUF[2]==0x00)
			{
				printf("\r\nɨ���豸��ϢΪ:\r\n");
				for(t=4;t<(len-2);t++)
				{
					USART_SendData(USART1, USART3_RX_BUF[t]);         //�򴮿�1��������
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
				}
				printf("\r\n\r\n");//���뻻��
				USART3_RX_STA=0;
				memset(USART3_RX_BUF,0,200);
				return 1;

			}
			else
			{
				printf("ɨ���豸ʧ��\r\n");
				USART3_RX_STA=0;
				memset(USART3_RX_BUF,0,200);
				return 0;
			}
	}
}

void mifare522Request(void)
{
	u16 len,t;
	requestCmd[5]=CalBCC(requestCmd,requestCmd[0]-2);
	print3RX(requestCmd,7);	
	while(!(USART3_RX_STA&0x8000));
	if(USART3_RX_STA&0x8000)
	{					   
			len=USART3_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			if(USART3_RX_BUF[2]==0x00)
			{
					printf("\r\n����ɹ�����ATQ:0x");
					for(t=4;t<(len-2);t++)
					{
						printf("%x",USART3_RX_BUF[t]);
					}
					printf("\r\n\r\n");//���뻻��

			}else
			{
					printf("\r\n����ʧ��\r\n");
			}
		USART3_RX_STA=0;
		memset(USART3_RX_BUF,0,200);
	}
}

void mifare522ReadID(void)
{
	u16 len,t;
	piccAnticollCmd[6]=CalBCC(piccAnticollCmd,piccAnticollCmd[0]-2);
	print3RX(piccAnticollCmd,8);
	while(!(USART3_RX_STA&0x8000));
	if(USART3_RX_STA&0x8000)
	{					   
			len=USART3_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			if(USART3_RX_BUF[2]==0x00)
			{
					printf("\r\n����ײ�ɹ����俨��:0x");
					for(t=(len-3);t>4;t--)
					{
						printf("%x",USART3_RX_BUF[t]);
					}
					printf("\r\n\r\n");//���뻻��
			}else
			{
					printf("\r\n����ʧ��\r\n");
			}
			USART3_RX_STA=0;
		  memset(USART3_RX_BUF,0,200);
	}
}





