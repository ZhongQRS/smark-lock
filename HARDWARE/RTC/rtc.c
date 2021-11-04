#include "rtc.h"

//����RTCʱ��
//������hour-Сʱ��min-���ӡ�sec-�롢ampm-��������
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{	
	RTC_TimeStructure.RTC_Hours=hour;
	RTC_TimeStructure.RTC_Minutes=min;
	RTC_TimeStructure.RTC_Seconds=sec;
	RTC_TimeStructure.RTC_H12=ampm;
	
	return RTC_SetTime(RTC_Format_BIN,&RTC_TimeStructure);
}

//����RTC����
//������year-�ꡢmonth-�¡�date-�ա�week-�ܼ�
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	RTC_DateTypeInitStructure.RTC_Date=date;
	RTC_DateTypeInitStructure.RTC_Month=month;
	RTC_DateTypeInitStructure.RTC_WeekDay=week;
	RTC_DateTypeInitStructure.RTC_Year=year;
	return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
}

//RTC�����Ի��Ѻ���
//���ܣ�ÿ�뻽��һ��
void RTC_Set_WakeUp(void)
{
	RTC_WakeUpCmd(DISABLE);
	
	//����ʱ��Դ��Ӳ��ʱ��Ƶ��Ϊ1Hz
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	
	//ֻ����һ�μ��� ÿ�����һ�� 1-1=0
	RTC_SetWakeUpCounter(0);
	
	RTC_WakeUpCmd(ENABLE);

	//�����жϵĴ�����ʽ�������ж�
	RTC_ITConfig(RTC_IT_WUT, ENABLE);
	
	RTC_ClearFlag(RTC_FLAG_WUTF);
	
	EXTI_ClearITPendingBit(EXTI_Line22);
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;			
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//RTC�ֲ�涨
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	//���ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//RTC��ʼ������
//����ֵ:	0,��ʼ���ɹ�;
u8 My_RTC_Init(void)
{
	//ʹ��rtc��Ӳ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	/*ʹ�ܵ�Դ����ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	/*�������RTC */
	PWR_BackupAccessCmd(ENABLE);
	
	//�ж��Ƿ��һ������?
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x9999)	
	{
		//��LSE��ʱ��
		RCC_LSEConfig(RCC_LSE_ON);

		//���LSE��ʱ���Ƿ����
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
		
		//ΪRTCѡ��LSE��Ϊʱ��Դ
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		
		/* ʹ��RTC��Ӳ��ʱ�� */
		RCC_RTCCLKCmd(ENABLE);
		
		/* �ȴ�����RTC��ؼĴ������� */
		RTC_WaitForSynchro();
		
		//����Ƶ��1Hz
		/* ck_spre(1Hz) = RTCCLK(LSE) /(uwAsynchPrediv + 1)*(uwSynchPrediv + 1)*/
		//32768Hz/(127+1)/(255+1)=1Hz
		RTC_InitStructure.RTC_AsynchPrediv = 127;
		RTC_InitStructure.RTC_SynchPrediv = 255;
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
		RTC_Init(&RTC_InitStructure);
		
		//����ʱ��20��33��05 am
		RTC_Set_Time(20,33,05,RTC_H12_AM);	
		
		//��������2020-03-23 ��һ
		RTC_Set_Date(20,3,23,1);		
		
		//�����Ի��Ѻ���
		RTC_Set_WakeUp();
	}
	return 0;
}

//��������A����
//���ܼ����ö����ǰ���������
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec)
{
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);//�ر�����A 
	
	RTC_TimeStructure.RTC_Hours=hour;//Сʱ
	RTC_TimeStructure.RTC_Minutes=min;//����
	RTC_TimeStructure.RTC_Seconds=sec;//��
	RTC_TimeStructure.RTC_H12=RTC_H12_AM;
  
	RTC_AlarmStructure.RTC_AlarmDateWeekDay=week;//����
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_WeekDay;//��������
	RTC_AlarmStructure.RTC_AlarmMask=RTC_AlarmMask_None;//��ȷƥ�����ڣ�ʱ����
	RTC_AlarmStructure.RTC_AlarmTime=RTC_TimeStructure;
	RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmStructure);
 
	
	RTC_ClearITPendingBit(RTC_IT_ALRA);//���RTC����A�ı�־
	EXTI_ClearITPendingBit(EXTI_Line17);//���LINE17�ϵ��жϱ�־λ 
	
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);//��������A�ж�
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);//��������A 
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;//LINE17
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE17
	EXTI_Init(&EXTI_InitStructure);//����

	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
}

//����1��ӡʱ�亯��
void USART1_Printf_Time(void)
{
	//RTC_GetTime����ȡʱ��
	RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure); 
	//printf("TIME:%02x:%02x:%02x\r\n",RTC_TimeStructure.RTC_Hours,RTC_TimeStructure.RTC_Minutes,RTC_TimeStructure.RTC_Seconds);
	
	//RTC_GetDate����ȡ����
	RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure);
	//printf("DATE:20%02x/%02x/%02x %0x\r\n",RTC_DateStructure.RTC_Year,RTC_DateStructure.RTC_Month,RTC_DateStructure.RTC_Date,RTC_DateStructure.RTC_WeekDay);		

	delay_ms(1000);
}

//RTC�����жϷ�����
void  RTC_WKUP_IRQHandler(void)
{
	//����־λ
	if(RTC_GetITStatus(RTC_IT_WUT) == SET)
	{
		//printf("RTC_WKUP_IRQHandler\r\n");
		
		//��ձ�־λ	
		RTC_ClearITPendingBit(RTC_IT_WUT);
		
		EXTI_ClearITPendingBit(EXTI_Line22);
	}
}

//RTC�����жϷ�����
void RTC_Alarm_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET)//ALARM A�ж�?
	{
		RTC_ClearFlag(RTC_FLAG_ALRAF);//����жϱ�־
		//printf("ALARM A!\r\n");
	}   
	EXTI_ClearITPendingBit(EXTI_Line17);	//����ж���17���жϱ�־ 											 
}
