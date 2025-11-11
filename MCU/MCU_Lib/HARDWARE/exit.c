#include "exit.h"
#include "delay.h"
#include "key.h"
#include "led.h"


void EXTIX_Init(void)
{
	EXTI_InitTypeDef Exti_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	KEY_Init();
	
//开启AFIO时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//设置IO口与中断线的映射关系、中断初始化、下降沿触发
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);//KEY1

	Exti_InitStructure.EXTI_Line  	= EXTI_Line3;
	Exti_InitStructure.EXTI_Mode  	= EXTI_Mode_Interrupt;
	Exti_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&Exti_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);//KEY0
	
	Exti_InitStructure.EXTI_Line  	= EXTI_Line4;
	EXTI_Init(&Exti_InitStructure);
	
//配置中断分组NVIC	
	NVIC_InitStructure.NVIC_IRQChannel  = EXTI3_IRQn; //KEY1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =  EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//外部中断3，负责 KEY1 按键检测，控制LED1
void EXTI3_IRQHandler(void)
{
	delay_ms(10);//消抖
	
	if(KEY1 == 0)
	{
		LED1 = !LED1;
	}
	
	EXTI_ClearITPendingBit(EXTI_Line3);//清除 LINE3 上的中断标志位
}

//外部中断4，负责 KEY0 按键检测，控制LED0
void EXTI4_IRQHandler(void)
{
	delay_ms(10);//消抖
	
	if(KEY0 == 0)
	{
		LED0 =  !LED0;
	}
	
	EXTI_ClearITPendingBit(EXTI_Line4);//清除 LINE3 上的中断标志位
} 
