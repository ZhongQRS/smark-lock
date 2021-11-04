#ifndef __PASSWORD_H
#define __PASSWORD_H

#include "stm32f4xx.h" 
#include "includes.h"
#include "usart.h"
#include "flash.h"
#include "delay.h"
#include "oled.h"

#define FLASH4 0x8010000 //����4

extern OS_Q	g_queue;//��Ϣ����

void OverWrite_Password(void);
u8 * Check_Password(char *buf);

#endif 
