#include "flash.h"
#include "stdio.h"

//����FLASH
void FLASH_erasure(void)
{
	//����FLASH�������FLASH�����޸�
	FLASH_Unlock();
	
	//����4����
	//��32bit�Ĵ�С��β����������
	if (FLASH_EraseSector(FLASH_Sector_4, VoltageRange_3) != FLASH_COMPLETE)
	{
		//printf("FLASH_EraseSector error\r\n");
		while(1);
	}
	//����FLASH���������FLASH�����޸�
	FLASH_Lock(); 
}

void FLASH_erasure_5(void)
{
	//����FLASH�������FLASH�����޸�
	FLASH_Unlock();
	
	//����4����
	//��32bit�Ĵ�С��β����������
	if (FLASH_EraseSector(FLASH_Sector_5, VoltageRange_3) != FLASH_COMPLETE)
	{
		//printf("FLASH_EraseSector error\r\n");
		while(1);
	}
	//����FLASH���������FLASH�����޸�
	FLASH_Lock(); 
}


void FLASH_erasure_6(void)
{
	//����FLASH�������FLASH�����޸�
	FLASH_Unlock();
	
	//����4����
	//��32bit�Ĵ�С��β����������
	if (FLASH_EraseSector(FLASH_Sector_6, VoltageRange_3) != FLASH_COMPLETE)
	{
		//printf("FLASH_EraseSector error\r\n");
		while(1);
	}
	//����FLASH���������FLASH�����޸�
	FLASH_Lock(); 
}



//д���֣�32λ��
void WriteFlashOneWord(uint32_t WriteAddress, uint32_t WriteData)
{
	//����FLASH�������FLASH�����޸�
	FLASH_Unlock();
	
	//���ݱ��
	//������4�׵�ַ0x8010000д������0x12345678
	if (FLASH_ProgramWord(WriteAddress, WriteData) != FLASH_COMPLETE)
	{
		//printf("FLASH_ProgramWord error\r\n");
		while(1);
	}
	
	//����FLASH���������FLASH�����޸�
	FLASH_Lock();
	

}


//д���ַ�����8λ��
void WriteFlashData(uint32_t WriteAddress, uint8_t data[], uint32_t num)
{
	uint32_t i = 0;
    uint16_t temp = 0;
	
	
	//����FLASH�������FLASH�����޸�
	FLASH_Unlock();
	
	
	for(i=0; i<num; i++)
	{
		temp = (uint16_t)data[i];
		if (FLASH_COMPLETE != FLASH_ProgramByte((WriteAddress+i), temp))//д������
		{
			//printf("FLASH_ProgramWord error\r\n");
			while(1);
		}
	}


	//����FLASH���������FLASH�����޸�
	FLASH_Lock();
}


//��ȡһ���֣�32λ��������
void ReadFlashOneWord(uint32_t WriteAddress,uint32_t num)
{
	uint32_t i = 0;
	uint32_t d=0;
	
	
	for(i=0;i<num*4;i=i+4)
	{
		//��ȡ����
		d = *(volatile uint32_t *)(WriteAddress+i);
		
		//printf("d=%X\r\n",d);	
	}
}

//��ȡ�ַ�����8λ��
void ReadFlashData(uint32_t WriteAddress,char *buf,uint32_t num)
{
	uint32_t i = 0;
	uint32_t d=0;

	for(i=0;i<num;i++)
	{
		//��ȡ����
		d = *(volatile uint32_t *)(WriteAddress+i);
		
		buf[i]=d;
	}
}
