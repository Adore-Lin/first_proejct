#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "exit.h"

/************************************************
 ALIENTEK 精英STM32F103开发板
 
实验8: 按键输入实验
将 STM32F1 的 IO 口作为外部中断输入
以中断的方式利用板载的 3 个按键， 来控制板载的两个 LED 的亮灭

************************************************/



 int main(void)
 {	
//	u8 key;
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 
   	LED_Init();
	KEY_Init();
	EXTIX_Init();

	LED0 = 0;
	 
  while(1)
	{
//	  key = KEY_Scan(0);
//		
//		if(key)
//		{
//			switch(key)
//			{
//				case KEY1_PRES:
//					LED1 =! LED1;
//					break;
//				case KEY0_PRES:
//					LED0 =! LED0;
//					break;
//			}
//		}
//		else
//			delay_ms(10);
	}
	
 }
