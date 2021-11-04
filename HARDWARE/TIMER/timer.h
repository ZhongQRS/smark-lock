#ifndef __TIMER_H
#define __TIMER_H

//////////////////////////////////////////////////////////////////////////////////	 
//						����GEC-STM32f407ZE������
//						��ʱ����������	   
//						���ߣ�unkim
//						��ϵ��ʽ��122078699@qq.com
//						��������:2020/3/21									  
//////////////////////////////////////////////////////////////////////////////////

#include "stm32f4xx.h"
#include "sys.h"

static NVIC_InitTypeDef	NVIC_InitStructure;
static TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

void tim1_init(void);
void tim2_init(void);
void tim3_init(void);
void tim8_init(void);

#endif
