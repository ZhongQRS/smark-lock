#ifndef __FLASH_H
#define	__FLASH_H

#include "stm32f4xx.h"


//��������
void FLASH_erasure(void);
void FLASH_erasure_5(void);
void FLASH_erasure_6(void);

//д�� 1�֣�32λ��
void WriteFlashOneWord(uint32_t WriteAddress, uint32_t WriteData);


//д���ַ���һ���ֽڣ�8λ��
void WriteFlashData(uint32_t WriteAddress, uint8_t data[], uint32_t num);


//��ȡnum�֣�32λ��
void ReadFlashOneWord(uint32_t WriteAddress,uint32_t num);


//��ȡnum�ֽڣ�8λ�������ַ�����
void ReadFlashData(uint32_t WriteAddress,char *buf,uint32_t num);

#endif
