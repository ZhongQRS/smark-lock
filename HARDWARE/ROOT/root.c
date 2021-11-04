#include "root.h"


void *ROOT_Mode(void)	//rootģʽ
{
	OS_ERR  err;
	OS_MSG_SIZE msg_size;
	uint8_t *p=NULL;
	
	OLED_Clear(); 	//OLED����
	OLED_ShowString(8,0,"1 ->Finger");
	OLED_ShowString(8,2,"2 ->RFID");
	OLED_ShowString(8,4,"3 ->Password");
	OLED_ShowString(8,6,"B ->Exit");
	
	p=OSQPend(&g_queue,0,OS_OPT_PEND_BLOCKING,&msg_size,NULL,&err);	//�ȴ���Ϣ����
	if(p != NULL)
	{
		if(strcmp((char*)p,"1")==0)//1-->¼ָ��
		{				
			OLED_Clear(); 	//OLED����
			OLED_ShowString(8,0,"Enroll FingerPrint");	
			//printf("¼ָ��\r\n");
			Add_FR();
			return NULL;
		}
		else if(strcmp((char*)p,"3")==0)//3-->��������
		{
			OLED_Clear(); 	//OLED����
			OLED_ShowString(8,0,"Overwrite Password");
			//printf("��д����\r\n");
			OverWrite_Password();
			return NULL;
		}
		else if(strcmp((char*)p,"B")==0)//B�˳�	B-->Back
		{
			OLED_Clear(); 	//OLED����
			OLED_ShowString(8,0,"Exit Root!");	
			//printf("�˳�rootģʽ\r\n");
			return NULL;
		}
	}
	
}
