#include "sys.h"
#include "delay.h"
#include "usart2.h"
#include "led.h"
#include "beep.h"
#include "oled.h"
#include "rtc.h"
#include "keyboard.h"
#include "includes.h"
#include "bmp.h"
#include "as608.h"
#include "timer.h"
#include "exti.h"
#include "root.h"
#include "password.h"
#include "flash.h"
#include "bluetooth.h"
//#include "mifare522.h"
#include "lock.h"

#define usart2_baund  57600//����2�����ʣ�����ָ��ģ�鲨���ʸ���

extern SysPara AS608Para;//ָ��ģ��AS608����
extern u16 ValidN;//ģ������Чָ�Ƹ���

static volatile uint8_t  usart_buf[32]={0};
static volatile uint32_t usart_cnt=0;


OS_Q		g_queue;		//��Ϣ����
OS_MUTEX	g_mutex;		//������
OS_FLAG_GRP	g_flag_grp;		//�¼���־��
OS_SEM	g_sem;					//�ź���

//��ʼ��������ƿ�
OS_TCB TaskInit_TCB;
void TaskInit(void *parg);
CPU_STK TaskInit_stk[128];			//��ʼ������������ջ����СΪ128�֣�Ҳ����512�ֽ�

//����1���ƿ�
OS_TCB Task1_TCB;
void task1(void *parg);
CPU_STK task1_stk[128];			//����1�������ջ����СΪ128�֣�Ҳ����512�ֽ�

//����2���ƿ�
OS_TCB Task2_TCB;
void task2(void *parg);
CPU_STK task2_stk[128];			//����2�������ջ����СΪ128�֣�Ҳ����512�ֽ�

//����3���ƿ�
OS_TCB Task3_TCB;
void task3(void *parg);
CPU_STK task3_stk[128];			//����3�������ջ����СΪ128�֣�Ҳ����512�ֽ�

//����4���ƿ�
OS_TCB Task4_TCB;
void task4(void *parg);
CPU_STK task4_stk[128];			//����4�������ջ����СΪ128�֣�Ҳ����512�ֽ�


//������
int main(void)
{
	OS_ERR err;
	systick_init();  													//ʱ�ӳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);						//�жϷ�������

	//OS��ʼ�������ǵ�һ�����еĺ���,��ʼ�����ֵ�ȫ�ֱ����������ж�Ƕ�׼����������ȼ����洢��
	OSInit(&err);

	//������ʼ������(��ʼ������)
	OSTaskCreate(	(OS_TCB *)&TaskInit_TCB,								//������ƿ飬��ͬ���߳�id
					(CPU_CHAR *)"TaskInit_TCB",								//��������֣����ֿ����Զ����
					(OS_TASK_PTR)TaskInit,									//����������ͬ���̺߳���
					(void *)0,												//���ݲ�������ͬ���̵߳Ĵ��ݲ���
					(OS_PRIO)4,											 	//��������ȼ�4	��� ����ִ��		
					(CPU_STK *)TaskInit_stk,								//�����ջ����ַ
					(CPU_STK_SIZE)128/10,									//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					(CPU_STK_SIZE)128,										//�����ջ��С			
					(OS_MSG_QTY)0,											//��ֹ������Ϣ����
					(OS_TICK)0,												//Ĭ��ʱ��Ƭ����																
					(void  *)0,												//����Ҫ�����û��洢��
					(OS_OPT)OS_OPT_TASK_NONE,								//û���κ�ѡ��
					&err													//���صĴ�����
				);
	
	//��������1
	OSTaskCreate(	&Task1_TCB,									//������ƿ飬��ͬ���߳�id
					"Task1",									//��������֣����ֿ����Զ����
					task1,										//����������ͬ���̺߳���
					0,											//���ݲ�������ͬ���̵߳Ĵ��ݲ���
					6,											//��������ȼ�6		
					task1_stk,									//�����ջ����ַ
					128/10,										//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					128,										//�����ջ��С			
					0,											//��ֹ������Ϣ����
					0,											//Ĭ������ռʽ�ں�															
					0,											//����Ҫ�����û��洢��
					OS_OPT_TASK_NONE,							//û���κ�ѡ��
					&err										//���صĴ�����
				);

	//��������2
	OSTaskCreate(	&Task2_TCB,									//������ƿ�
					"Task2",									//���������
					task2,										//������
					0,											//���ݲ���
					7,											//��������ȼ�7		
					task2_stk,									//�����ջ����ַ
					128/10,										//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					128,										//�����ջ��С			
					0,											//��ֹ������Ϣ����
					0,											//Ĭ������ռʽ�ں�																
					0,											//����Ҫ�����û��洢��
					OS_OPT_TASK_NONE,							//û���κ�ѡ��
					&err										//���صĴ�����
				);

	//��������3
	OSTaskCreate(	&Task3_TCB,									//������ƿ�
					"Task3",									//���������
					task3,										//������
					0,											//���ݲ���
					7,											//��������ȼ�7		
					task3_stk,									//�����ջ����ַ
					128/10,										//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					128,										//�����ջ��С			
					0,											//��ֹ������Ϣ����
					0,											//Ĭ������ռʽ�ں�																
					0,											//����Ҫ�����û��洢��
					OS_OPT_TASK_NONE,							//û���κ�ѡ��
					&err										//���صĴ�����
				);
			
	//��������4
	OSTaskCreate(	&Task4_TCB,									//������ƿ�
					"Task4",									//���������
					task4,										//������
					0,											//���ݲ���
					7,											//��������ȼ�7		
					task4_stk,									//�����ջ����ַ
					128/10,										//�����ջ�����λ���õ����λ�ã��������ټ���ʹ��
					128,										//�����ջ��С			
					0,											//��ֹ������Ϣ����
					0,											//Ĭ������ռʽ�ں�																
					0,											//����Ҫ�����û��洢��
					OS_OPT_TASK_NONE,							//û���κ�ѡ��
					&err										//���صĴ�����
				);

	OSQCreate(&g_queue,"g_queue",64,&err);		//������Ϣ����		
	OSMutexCreate(&g_mutex,"g_mutex",&err);		//����������
	OSSemCreate (&g_sem,"g_sem",0,&err);			//�����ź���,��ֵΪ0	
	OSFlagCreate(&g_flag_grp,"g_flag_grp",0,&err);//��Ϣ��־��

	//����OS�������������
	OSStart(&err);					
	//printf("never run.......\r\n");
					
	while(1);
}



void TaskInit(void *parg)	//��ʼ��Ӳ������
{
	OS_ERR err;
	u8 ensure;
	OSTaskSuspend(&Task1_TCB,&err);	//����1����
	OSTaskSuspend(&Task2_TCB,&err);	//����2����
	OSTaskSuspend(&Task3_TCB,&err);	//����3����
	OSTaskSuspend(&Task4_TCB,&err);	//����4����
	
	LED_Init();         		//LED��ʼ��	
	BEEP_Init();  				//��������ʼ��
	usart_init(9600);  		//����1����9600 \ usb����115200
	usart2_init(usart2_baund);	//ָ�ƴ���2
	EXTI9_Init();				//ָ���ж�---PG9
	KEYBOARD_Init();			//���̳�ʼ��	
	My_RTC_Init();				//ʱ���ʼ��
	OLED_Init();				//OLED��ʼ��  
	OLED_Clear(); 				//OLED����
	//uart3_init(9600);			//����3RFID��ʼ��
	//LOCK_Init();				//����ʼ��
	
	//printf("lnited\r\n");
	OLED_ShowString(8,0,"Init Successed!");  
	delay_ms(500);

	//printf("stm32����ָ����\r\n");
	//printf("by��unkim\r\n");
	//printf("��AS608ģ������....\r\n");
	
	OLED_Clear();
	OLED_ShowString(24,2,"Smart lock");  
	OLED_ShowString(24,4,"by: unkim");  
	delay_ms(2000);
	OLED_Clear(); 
	
	while(PS_HandShake(&AS608Addr))//��AS608ģ������
	{
		delay_ms(400);
		OLED_ShowString(16,2,"connecting...");
		//printf("��������\n");
		delay_ms(400);
	}
	
	OLED_Clear();
	//printf("���ӳɹ�\r\n");
	OLED_ShowString(16,2,"connected");
	delay_ms(1000);
	
	OLED_Clear(); 
	//printf("������:%d   ��ַ:%x\r\n",usart2_baund,AS608Addr);
	
	ensure=PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
	if(ensure!=0x00)
		ShowErrMessage(ensure);//��ʾȷ���������Ϣ
	ensure=PS_ReadSysPara(&AS608Para);  //������
	if(ensure==0x00)
	{
		//printf("ָ�ƿ�����:%d     �Աȵȼ�: %d\r\n",AS608Para.PS_max-ValidN,AS608Para.PS_level);
		OLED_ShowCHinese(0,2,0);//��
		OLED_ShowCHinese(16,2,1);//��
		OLED_ShowCHinese(32,2,2);//��
		OLED_ShowCHinese(48,2,7);//��
		OLED_ShowNum(64,2,AS608Para.PS_max-ValidN,3,16);
		
		OLED_ShowCHinese(0,4,3); //��
		OLED_ShowCHinese(16,4,4);//��
		OLED_ShowCHinese(32,4,5);//��
		OLED_ShowCHinese(48,4,6);//��
		OLED_ShowCHinese(64,4,7);//��
		OLED_ShowNum(80,4,AS608Para.PS_level,1,16);
	}
	else
		ShowErrMessage(ensure);
	delay_ms(1000);
	OSTaskResume(&Task1_TCB,&err);//����1�ָ�
	OSTaskResume(&Task2_TCB,&err);//����2�ָ�
	OSTaskResume(&Task3_TCB,&err);//����3�ָ�
	OSTaskResume(&Task4_TCB,&err);//����4�ָ�
	
	OSTaskDel(NULL,&err);//ɾ����ǰ����
	while(1);
}


void task1(void *parg)	//ģʽ����������
{
	OS_ERR  err;
	OS_MSG_SIZE msg_size;
	char *bp=NULL;
	uint8_t *p=NULL;
	char buf[10];
	memset(buf, 0,10);
	OSFlagPost(&g_flag_grp,0x01,OS_OPT_POST_FLAG_SET,&err);  //�¼���־0x01 ---> ����ʱ������
	
	//printf("task1-Model and function is create ok\r\n");
	
	while(1)
	{
		OSMutexPost(&g_mutex,OS_OPT_POST_NONE,&err);						//�������ͷ�OLED��Դ
		p=OSQPend(&g_queue,0,OS_OPT_PEND_BLOCKING,&msg_size,NULL,&err);		//�ȴ���Ϣ����
		if(p != NULL)	//����Ϣ����
		{
			OSMutexPend(&g_mutex,0,OS_OPT_PEND_BLOCKING,NULL,&err);			//����������OLED��Դ
			if(strlen(buf)<=2 && (strcmp((char *)p,"A")==0 || strcmp((char *)p,"C")==0))
			{
				strcat(buf,(char *)p);
			}	
			if(strcmp(buf,"ACC")==0)	//����ACC ����rootģʽ	ACC-->Account
			{
				//printf("enter root!\r\n");
				OLED_Clear(); 	//OLED����
				OLED_ShowString(8,2,"Enter Root!");	
				delay_ms(2000);
				p = (uint8_t *)ROOT_Mode();		//rootģʽ����
				memset(buf, 0,10);
				//printf("buf: %s\r\n",buf);
			}
			else if(strcmp((char*)p,"FingerPrint")==0)	//��ָ֤��
			{
				p=press_FR();//ˢָ��
				//printf("finger: %s\r\n",p);
				if(strcmp((char *)p,"success")==0)
				{
					//printf("ˢָ�Ƴɹ�������\r\n");
					//printf("finger: %s\r\n",p);
					usart_send_str("FingerPrint unlock!\n");
					OSSemPost(&g_sem,OS_OPT_POST_1,&err);//S=S+1=1����ʱV����//�ͷ��ź���//����
					OSTaskResume(&Task4_TCB,&err);//����4�ָ�
				}
			}
			else if(strcmp((char*)p,"Bluetooth")==0)
			{
				//"PW-123456-unkim"
				if(strstr((const char *)usart_buf,"PW"))
				{
					//�ֽ��ַ���,p="PW"
					bp=strtok((char *)usart_buf,"-");
					//�ֽ��ַ���,p="123456"
					bp=strtok(NULL,"-");
					p = Check_Password(bp);//У������
					if(strcmp((char *)p,"success")==0)
					{
						//printf("\n����������ȷ������\r\n");
						BEEP=1;
						delay_ms(500);
						BEEP=0;
						//memset(bp, 0,sizeof(bp));
						//memset(p, 0,sizeof(p));
						memset(p, 0,sizeof(usart_buf));
						usart_cnt=0;
						//printf("bluetooth p: %s\r\n",p);
						usart_send_str("Bluetooth unlock!\n");
						OSSemPost(&g_sem,OS_OPT_POST_1,&err);//S=S+1=1����ʱV����//�ͷ��ź���//����
						OSTaskResume(&Task4_TCB,&err);//����4�ָ�
						
					}
					else
					{
						//printf("\n�����������\r\n");
						//printf("bluetooth: %s\r\n",p);
						OLED_Clear(); 	//OLED����
						OLED_ShowString(24,3,"close");						
						memset(buf, 0,10);
						delay_ms(2000);
						usart_cnt=0;
						
					}
				}
			}
			else if(strcmp((char *)p,"A")!=0 && strcmp((char *)p,"C")!=0)	//��֤���� �ų�AC����
			{
				strcat(buf,(char *)p);
				while(1)
				{
					p=OSQPend(&g_queue,0,OS_OPT_PEND_BLOCKING,&msg_size,NULL,&err);		//�ȴ���Ϣ����
					//printf("���������룺\r\n");
					if(strcmp((char *)p,"B")==0)//B�˳�	B-->Back
					{
						break;
					}
					else if(strlen(buf)<=5)
					{
						strcat(buf,(char *)p);
						//printf("buf: %s\r\n",buf);
						OLED_Clear(); 	//OLED����
						OLED_ShowString(30,2,"password:");//��ʾ����
						OLED_ShowString(30,6,(u8 *)buf);//��ʾ����
					}
					else 
					{
						//printf("password: %s\r\n",buf);
						p = Check_Password(buf);//У������
						if(strcmp((char *)p,"success")==0)
						{
							//printf("����������ȷ������\r\n");
							BEEP=1;
							delay_ms(500);
							BEEP=0;
							memset(buf, 0,10);
							usart_send_str("Password unlock!\n");
							OSSemPost(&g_sem,OS_OPT_POST_1,&err);//S=S+1=1����ʱV����//�ͷ��ź���//����
							OSTaskResume(&Task4_TCB,&err);//����4�ָ�
							break;
						}
						else
						{
							//printf("�����������\r\n");
							OLED_Clear(); 	//OLED����
							OLED_ShowString(24,3,"close");						
							memset(buf, 0,10);
							break;
						}
					}
				
						
				}
					
			}
		}
	}
}

void task2(void *parg)	//��������
{	
	//printf("task2-keyboard is create ok\r\n");
	while(1)
	{
		KEYBOARD_Num();
	}
}

void task3(void *parg)	//ʱ������
{	
	OS_ERR  err;
	u8 time[20];
	u8 date[20];
	OSFlagPend(&g_flag_grp,0x01,0, OS_OPT_PEND_FLAG_SET_ANY+OS_OPT_PEND_BLOCKING,NULL,&err);	//ʱ�乤����־λ --> 0x01
	//printf("task3-time is create ok\r\n");
	
	while(1)
	{
		delay_ms(1000);
		RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);//��ȡʱ��
		RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);//��ȡ����
	
		OSMutexPend(&g_mutex,0,OS_OPT_PEND_BLOCKING,NULL,&err);//�ȴ������������ȴ��ɹ���������������Դ
		
		OLED_Clear(); //OLED����
		sprintf((char*)time,"%02d:%02d:%02d",RTC_TimeStructure.RTC_Hours,RTC_TimeStructure.RTC_Minutes,RTC_TimeStructure.RTC_Seconds);
		OLED_ShowString(35,3,time);//��ʾʱ��
		sprintf((char*)date,"%02d",RTC_DateStructure.RTC_Month);
		OLED_ShowString(37,0,date);
		OLED_ShowCHinese(53,0,8);//��ʾ��
		sprintf((char*)date,"%02d",RTC_DateStructure.RTC_Date);
		OLED_ShowString(69,0,date);
		OLED_ShowCHinese(85,0,9);//��ʾ��
		OLED_ShowCHinese(44,6,10);//��
		OLED_ShowCHinese(60,6,11);//��
		OLED_ShowNum(80,6,RTC_DateStructure.RTC_WeekDay,1,16);//��ʾ����
		
		OSMutexPost(&g_mutex,OS_OPT_POST_NONE,&err);//�ͷŻ�����������
	}
	
}

void task4(void *parg)	//������
{	
	OS_ERR  err;
	OSFlagPend(&g_flag_grp,0x01,0, OS_OPT_PEND_FLAG_SET_ANY+OS_OPT_PEND_BLOCKING,NULL,&err);//����������־λ
	LOCK_Init();	//����ʼ��
	while(1)
	{
		OSSemPend(&g_sem,0,OS_OPT_PEND_BLOCKING,NULL,&err);	//�õ���Դ��S=S-1=0����ʱP����//�ȴ��ź���
		//OSTaskSuspend(&Task3_TCB,&err);	//����3ʱ�����
		OLED_Clear(); 	//OLED����
		OLED_ShowString(8,2,"open");
		LOCK_ON();
		delay_ms(2000);
		LOCK_OFF();
		//OSTaskResume(&Task3_TCB,&err);//����3ʱ��ָ�
		OSTaskSuspend(&Task4_TCB,&err);	//����4����
	}
}


void EXTI9_5_IRQHandler(void)	//���PG9ָ�ƴ����жϷ�����
{
	OS_ERR  err;
	//����Ƿ��ж���0�Ѿ������ж�
	if(EXTI_GetITStatus(EXTI_Line9) == SET)
	{
		//�жϴ����¼�
		if(KeyBOARD_Scan(GPIOG,GPIO_Pin_9))
		{
			OSQPost(&g_queue,"FingerPrint",strlen("FingerPrint"),OS_OPT_POST_FIFO,&err);
		}
	}
	//����жϱ�־λ�������´��ж�����
	EXTI_ClearITPendingBit(EXTI_Line9);
}

//��������1�жϷ�����
void  USART1_IRQHandler(void)
{
	OS_ERR  err;
	uint8_t d;
	
	OSIntEnter(); 

	//����־λ
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		//��������
		d = USART_ReceiveData(USART1);
		
		usart_buf[usart_cnt] =d ;
		usart_cnt++;
		
		if(d == '#' || usart_cnt >= sizeof usart_buf)
		{
			OSQPost(&g_queue,"Bluetooth",strlen("Bluetooth"),OS_OPT_POST_FIFO,&err);
		}
		
		//��ձ�־λ	
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}

	OSIntExit();  
}







