#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "exit.h"
#include "usart.h"
#include "timer.h"

/************************************************
ALIENTEK 精英STM32F103开发板

驱动：
2个LED		已验证
2个按键key 	已验证
8: 按键输入实验：将 STM32F1 的 IO 口作为外部中断输入，以中断的方式利用板载的 3 个按键， 来控制板载的两个 LED 的亮灭

13：定时器中断 实验：使用 TIM3 的定时器中断来控制 DS1 的翻转，在主函数中用 DS0 的翻转来提示程序正在运行
14：定时器输出 PWM实验：使用 TIM3 的通道2，把通道2重映射到PB5，产生PWM来控制DS0的亮度
15: 定时器输入 捕获实验：使用 TIM5 的通道1（PA0） 来做输入捕获， 捕获 PA0 上高电平的脉宽（用 WK_UP 按键输入高电平），通过串口打印高电平脉宽时间，

************************************************/

void Usart_Test(void)
{
	u8 t;
	u16 len;
	u16  times = 0;

	if(USART_RX_STA & 0x8000)
	{
		len = USART_RX_STA & 0x3fff;
		printf("\r\n, 你发送的消息为：\r\n\r\n");
		for(t= 0; t < len; t++)
		{
			USART_SendData(USART1, USART_RX_BUF[t]);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
		}

		printf("\r\n\r\n");

		USART_RX_STA = 0;
	}
	else
	{
		times ++;
		if(times % 5000 == 0)
		{
			printf("\r\n Usart Test \r\n");
		}

		if(times % 200 == 0)
			printf("Please  input date \r\n");

		if(times % 30 == 0)
			LED0 = !LED0;

		delay_ms(10);
	}
}



void Hardware_Init(void)
{
	LED_Init();
	KEY_Init();
	EXTIX_Init();
	uart_init(115200);
	//TIM3_Int_Init(4999, 7199);//中断溢出时间Tout = (4999 + 1)*(7199 + 1)/72 = 500000us = 500ms
	TIM3_PWM_Init(899, 0);//不分频，PWM频率 = 72000/900 = 80Khz
}

int main(void)
{	
	//变量初始化
	u16 led0_pwm = 0;
	u8 dir = 1;
	
	//函数初始化
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 
	//驱动初始化
	Hardware_Init();

	 
	while(1)
	{
		
		
	}
}

